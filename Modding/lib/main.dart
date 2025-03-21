import 'dart:ui';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'application.dart';
import 'service/android_helper.dart';
import 'service/notification_helper.dart';
import 'service/ui_helper.dart';
import 'service/windows_helper.dart';
import 'package:window_manager/window_manager.dart';
import 'extension/platform.dart';

Future<void> main(List<String> arguments) async {
  WidgetsFlutterBinding.ensureInitialized();
  await SystemChrome.setEnabledSystemUIMode(SystemUiMode.edgeToEdge);
  if (CurrentPlatform.isDesktop) {
    await WindowManager.instance.ensureInitialized();
    await windowManager.setTitleBarStyle(TitleBarStyle.normal);
    await windowManager.setMinimumSize(const Size(1280, 720));
    await windowManager.center();
    await windowManager.waitUntilReadyToShow();
    await windowManager.show();
    await windowManager.focus();
  }
  await NotificationHelper.initialize();
  if (CurrentPlatform.isAndroid) {
    AndroidHelper.initialize();
  }
  if (CurrentPlatform.isWindows) {
    WindowsHelper.initialize();
  }
  final GlobalKey<NavigatorState> navigatorKey = GlobalKey<NavigatorState>();
  onHandleException({required String message, required StackTrace? stack}) {
    WidgetsBinding.instance.addPostFrameCallback((_) async {
      await UIHelper.showSimpleDialog(
        context: navigatorKey.currentState!.context,
        title: message,
        content: stack.toString(),
      );
    });
  }

  FlutterError.onError = (FlutterErrorDetails details) {
    onHandleException(
      message: details.exceptionAsString(),
      stack: details.stack,
    );
  };
  PlatformDispatcher.instance.onError = (Object error, StackTrace stack) {
    onHandleException(message: error.toString(), stack: stack);
    return true;
  };
  runApp(Application(navigatorKey: navigatorKey));
}
