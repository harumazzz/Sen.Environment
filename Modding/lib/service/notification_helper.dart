import 'dart:io';
import 'package:flutter/foundation.dart';
import 'package:path/path.dart' as path;
import 'package:flutter_local_notifications/flutter_local_notifications.dart';
import '../constant/build_distribution.dart';

class NotificationHelper {
  const NotificationHelper._();

  static FlutterLocalNotificationsPlugin? _flutterLocalNotificationsPlugin;

  static String get _iconPath {
    if (kDebugMode) {
      return path.join(Directory.current.path, 'assets', 'images', 'logo.ico');
    } else {
      final exePath = Platform.resolvedExecutable;
      final assetsPath = path.join(
        path.dirname(exePath),
        'data',
        'flutter_assets',
        'assets',
        'images',
        'logo.ico',
      );
      return assetsPath;
    }
  }

  static Future<void> initialize() async {
    _flutterLocalNotificationsPlugin = FlutterLocalNotificationsPlugin();
    final initializationSettings = InitializationSettings(
      android: const AndroidInitializationSettings('@mipmap/launcher_icon'),
      iOS: const DarwinInitializationSettings(),
      macOS: const DarwinInitializationSettings(),
      linux: LinuxInitializationSettings(
        defaultActionName: '',
        defaultIcon: AssetsLinuxIcon('assets/images/logo.png'),
      ),
      windows: WindowsInitializationSettings(
        appName: BuildDistribution.kApplicationName,
        appUserModelId: 'com.haruma.sen.environment',
        guid: '462ebc1c-00e6-4f29-9854-38407451743a',
        iconPath: _iconPath,
      ),
    );
    await _flutterLocalNotificationsPlugin!.initialize(initializationSettings);
    return;
  }

  static Future<void> push(String title, String description) async {
    await _flutterLocalNotificationsPlugin!.show(
      DateTime.now().millisecondsSinceEpoch % 0x80000000,
      title,
      description,
      const NotificationDetails(
        android: AndroidNotificationDetails(
          'com.haruma.sen.environment.notification_channel.main',
          'Main',
        ),
        iOS: DarwinNotificationDetails(),
        macOS: DarwinNotificationDetails(),
        linux: LinuxNotificationDetails(),
        windows: WindowsNotificationDetails(),
      ),
    );
    return;
  }
}
