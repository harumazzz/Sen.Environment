import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:sen/application.dart';
import 'package:sen/constant/system_overlay.dart';
import 'package:sen/service/android_helper.dart';
import 'package:sen/service/notification_helper.dart';
import 'package:sen/service/windows_helper.dart';
import 'package:window_manager/window_manager.dart';
import 'package:sen/extension/platform.dart';

Future<void> main(List<String> arguments) async {
  WidgetsFlutterBinding.ensureInitialized();
  SystemChrome.setEnabledSystemUIMode(SystemUiMode.edgeToEdge);
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
  if (CurrentPlatform.isAndroid) {
    AndroidHelper.initialize();
  }
  if (CurrentPlatform.isWindows) {
    WindowsHelper.initialize();
  }
  runApp(const Application());
}
