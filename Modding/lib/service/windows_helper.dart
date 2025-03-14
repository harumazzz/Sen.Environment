import 'package:flutter/services.dart';

class WindowsHelper {
  const WindowsHelper._();

  static late final MethodChannel _methodChannel;

  static void initialize() {
    _methodChannel = const MethodChannel('com.haruma.sen.environment');
  }

  static Future<void> runLauncher({required String argument}) async {
    try {
      await _methodChannel.invokeMethod('run_launcher', {'argument': argument});
    } on PlatformException catch (e) {
      throw Exception(e.message);
    }
    return;
  }
}
