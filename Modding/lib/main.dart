import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'application.dart';
import 'constant/system_overlay.dart';
import 'service/android_helper.dart';
import 'service/notification_helper.dart';
import 'service/windows_helper.dart';
import 'package:window_manager/window_manager.dart';
import 'extension/platform.dart';

Future<void> main(List<String> arguments) async {
  WidgetsFlutterBinding.ensureInitialized();
  await SystemChrome.setEnabledSystemUIMode(SystemUiMode.edgeToEdge);
  SystemChrome.setSystemUIOverlayStyle(SystemOverlay.kDefaultOverlay);
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
  await NotificationHelper.push('Sen', 'Hi');
  if (CurrentPlatform.isAndroid) {
    AndroidHelper.initialize();
  }
  if (CurrentPlatform.isWindows) {
    WindowsHelper.initialize();
  }
  runApp(const Application());
}
