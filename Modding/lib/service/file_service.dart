import 'dart:convert';
import 'dart:io';
import 'dart:typed_data';
import 'package:file_selector/file_selector.dart' as file_selector;
import 'package:sen/service/android_service.dart';
import 'package:path/path.dart' as p;
import 'package:path_provider/path_provider.dart' as path_provider;

class FileService {
  static List<String> readDirectory({
    required String source,
    required bool recursive,
  }) {
    final dir = Directory(source);
    final list = dir.listSync(recursive: recursive);
    List<String> result = [];
    for (var ripe in list) {
      result.add(ripe.path);
    }
    return result;
  }

  static Future<List<String>> readDirectoryAsync({
    required String source,
    required bool recursive,
  }) async {
    final dir = Directory(source);
    List<String> result = [];
    await for (var entity in dir.list(recursive: recursive)) {
      result.add(entity.path);
    }
    return result;
  }

  static Future<String?> saveFile({
    String? suggestedName,
  }) async {
    var outputFile = null as String?;
    if (Platform.isWindows ||
        Platform.isLinux ||
        Platform.isMacOS ||
        Platform.isIOS) {
      outputFile = (await file_selector.getSaveLocation(
        suggestedName: suggestedName,
      ))
          ?.path;
    }
    if (Platform.isAndroid) {
      outputFile = (await AndroidService.saveFileFromDocument());
    }
    return outputFile != null ? p.absolute(outputFile) : null;
  }

  static bool isDirectory(String source) {
    return Directory(source).existsSync();
  }

  static bool isFile(String source) {
    return File(source).existsSync();
  }

  static void writeFile({
    required String source,
    required String data,
  }) {
    final file = File(source);
    file.writeAsStringSync(data);
    return;
  }

  static void writeJson({
    required String source,
    required dynamic data,
  }) {
    final file = File(source);
    file.writeAsStringSync(const JsonEncoder.withIndent('\t').convert(data));
    return;
  }

  static String readFile({
    required String source,
  }) {
    return File(source).readAsStringSync();
  }

  static Uint8List readBuffer({
    required String source,
  }) {
    var file = File(source);
    return file.readAsBytesSync();
  }

  static Future<Uint8List> readBufferAsync({
    required String source,
  }) async {
    var file = File(source);
    return await file.readAsBytes();
  }

  static void writeBuffer({
    required String source,
    required Uint8List data,
  }) {
    var file = File(source);
    file.writeAsBytesSync(data);
    return;
  }

  static dynamic readJson({
    required String source,
  }) {
    return jsonDecode(readFile(source: source));
  }

  static Future<String?> uploadDirectory() async {
    var directory = null as String?;
    if (Platform.isAndroid) {
      directory = await AndroidService.pickDirectoryFromDocument();
    } else {
      directory = (await file_selector.getDirectoryPath());
    }
    if (directory == null || directory.isEmpty) {
      return null;
    }
    return directory;
  }

  static Future<String?> uploadFile() async {
    if (Platform.isAndroid) {
      return await AndroidService.pickFileFromDocument();
    }
    return await _uploadFilePicker();
  }

  static Future<String?> _uploadFilePicker() async {
    var result = (await file_selector.openFile())?.path;
    if (result == null) {
      return null;
    } else {
      return p.absolute(result);
    }
  }

  static Future<String> getWorkingDirectory() async {
    if (Platform.isAndroid) {
      return (await path_provider.getExternalStorageDirectory())!.path;
    }
    if (Platform.isIOS) {
      return (await path_provider.getApplicationDocumentsDirectory()).path;
    }
    if (Platform.isWindows) {
      return p.absolute(
          (await path_provider.getApplicationSupportDirectory()).path);
    }
    return (await path_provider.getApplicationSupportDirectory()).path;
  }
}
