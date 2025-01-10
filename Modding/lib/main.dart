import 'dart:io';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:sen/application.dart';
import 'package:sen/service/android_helper.dart';
import 'package:sen/service/notification_helper.dart';
import 'package:sen/service/windows_helper.dart';
import 'package:window_manager/window_manager.dart';

Future<void> main(
  List<String> arguments,
) async {
  WidgetsFlutterBinding.ensureInitialized();
  if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
    await windowManager.setTitleBarStyle(TitleBarStyle.normal);
    await WindowManager.instance.ensureInitialized();
    await windowManager.setMinimumSize(const Size(1280, 720));
    await windowManager.center();
    await windowManager.waitUntilReadyToShow();
    await windowManager.show();
    await windowManager.focus();
  }
  await NotificationHelper.initialize();
  if (Platform.isAndroid) {
    AndroidHelper.initialize();
  }
  if (Platform.isWindows) {
    WindowsHelper.initialize();
  }
  runApp(
    const ProviderScope(
      child: Application(),
    ),
  );
}
