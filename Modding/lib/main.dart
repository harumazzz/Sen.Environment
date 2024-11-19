import 'dart:io';
import 'package:firebase_core/firebase_core.dart';
import 'package:flutter/material.dart';
import 'package:flutter_dotenv/flutter_dotenv.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:sen/application.dart';
import 'package:sen/firebase_options.dart';
import 'package:sen/service/android_service.dart';
import 'package:sen/service/connection_service.dart';
import 'package:sen/service/notification_service.dart';
import 'package:window_manager/window_manager.dart';

Future<void> main(
  List<String> arguments,
) async {
  WidgetsFlutterBinding.ensureInitialized();
  await dotenv.load(fileName: ".env");
  if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
    await windowManager.setTitleBarStyle(TitleBarStyle.normal);
    await WindowManager.instance.ensureInitialized();
    await windowManager.setMinimumSize(const Size(1280, 720));
    await windowManager.center();
    await windowManager.waitUntilReadyToShow();
    await windowManager.show();
    await windowManager.focus();
  }
  await NotificationService.initialize();
  await ConnectionService.initialize();
  if (Platform.isAndroid) {
    AndroidService.initialize();
  }
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );
  runApp(
    const ProviderScope(
      child: Application(),
    ),
  );
}
