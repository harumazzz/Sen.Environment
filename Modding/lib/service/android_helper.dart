import 'package:flutter/services.dart';

class AndroidHelper {
  const AndroidHelper._();
  static late final MethodChannel _methodChannel;

  static List<String>? _arguments;

  static List<String>? get arguments => _arguments;

  static void initialize() {
    _methodChannel = const MethodChannel('com.haruma.sen.environment');
    _methodChannel.setMethodCallHandler(_handleMethod);
  }

  static Future<void> _handleMethod(MethodCall call) async {
    switch (call.method) {
      case 'onResourcesReceived':
        _arguments = List<String>.from(call.arguments);
        break;
      default:
        throw UnimplementedError();
    }
  }

  static Future<bool> requestStoragePermission() async {
    final result = await _methodChannel.invokeMethod<bool>(
      'request_storage_permission',
    );
    return result!;
  }

  static Future<bool> checkStoragePermission() async {
    final result = await _methodChannel.invokeMethod<bool>(
      'check_storage_permission',
    );
    return result!;
  }

  static Future<String?> pickFileFromDocument(String? initialDirectory) async {
    final result = await _methodChannel.invokeMethod<String?>('pick_file', {
      'initialDirectory': initialDirectory,
    });
    return result;
  }

  static Future<String?> pickDirectoryFromDocument(
    String? initialDirectory,
  ) async {
    final result = await _methodChannel.invokeMethod<String?>(
      'pick_directory',
      {'initialDirectory': initialDirectory},
    );
    return result;
  }

  static Future<String?> saveFileFromDocument() async {
    final result = await _methodChannel.invokeMethod<String?>('save_file');
    return result;
  }
}
