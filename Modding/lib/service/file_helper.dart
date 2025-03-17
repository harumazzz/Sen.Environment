import 'dart:convert';
import 'dart:io';
import 'dart:typed_data';
import 'package:file_selector/file_selector.dart' as file_selector;
import '../extension/platform.dart';
import 'android_helper.dart';
import 'package:path/path.dart' as p;
import 'package:path_provider/path_provider.dart' as path_provider;
import 'package:archive/archive.dart';
import 'package:url_launcher/url_launcher.dart';

class FileHelper {
  const FileHelper._();

  static List<String> readDirectory({
    required String source,
    required bool recursive,
  }) {
    final dir = Directory(source);
    final list = dir.listSync(recursive: recursive);
    List<String> result = [];
    for (final ripe in list) {
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
    await for (final entity in dir.list(recursive: recursive)) {
      result.add(entity.path);
    }
    return result;
  }

  static Future<String?> saveFile({
    String? suggestedName,
    String? initialDirectory,
  }) async {
    var outputFile = null as String?;
    if (CurrentPlatform.isDesktop || CurrentPlatform.isIOS) {
      outputFile =
          (await file_selector.getSaveLocation(
            suggestedName: suggestedName,
            initialDirectory: initialDirectory,
          ))?.path;
    }
    if (CurrentPlatform.isAndroid) {
      outputFile = await AndroidHelper.saveFileFromDocument();
    }
    return outputFile != null ? p.absolute(outputFile) : null;
  }

  static bool isDirectory(String source) {
    return Directory(source).existsSync();
  }

  static bool isFile(String source) {
    return File(source).existsSync();
  }

  static void writeFile({required String source, required String data}) {
    final file = File(source);
    file.writeAsStringSync(data);
    return;
  }

  static void writeJson({required String source, required dynamic data}) {
    final file = File(source);
    file.writeAsStringSync(const JsonEncoder.withIndent('\t').convert(data));
    return;
  }

  static Future<void> writeJsonAsync({
    required String source,
    required dynamic data,
  }) async {
    final file = File(source);
    await file.writeAsString(
      const JsonEncoder.withIndent('\t').convert(data),
      flush: true,
    );
    return;
  }

  static String readFile({required String source}) {
    return File(source).readAsStringSync();
  }

  static Future<String> readFileAsync({required String source}) async {
    return await File(source).readAsString();
  }

  static Uint8List readBuffer({required String source}) {
    var file = File(source);
    return file.readAsBytesSync();
  }

  static Future<Uint8List> readBufferAsync({required String source}) async {
    var file = File(source);
    return await file.readAsBytes();
  }

  static void writeBuffer({required String source, required Uint8List data}) {
    var file = File(source);
    file.writeAsBytesSync(data);
    return;
  }

  static dynamic readJson({required String source}) {
    return jsonDecode(readFile(source: source));
  }

  static Future<dynamic> readJsonAsync({required String source}) async {
    return jsonDecode(await readFileAsync(source: source));
  }

  static Future<String?> uploadDirectory({String? initialDirectory}) async {
    var directory = null as String?;
    if (CurrentPlatform.isAndroid) {
      directory = await AndroidHelper.pickDirectoryFromDocument(
        initialDirectory,
      );
    } else {
      directory = await file_selector.getDirectoryPath(
        initialDirectory: initialDirectory,
      );
    }
    if (directory == null || directory.isEmpty) {
      return null;
    }
    return directory;
  }

  static Future<String?> uploadFile({String? initialDirectory}) async {
    if (CurrentPlatform.isAndroid) {
      return await AndroidHelper.pickFileFromDocument(initialDirectory);
    }
    return await _uploadFilePicker(initialDirectory);
  }

  static Future<String?> _uploadFilePicker(String? initialDirectory) async {
    final result =
        (await file_selector.openFile(
          initialDirectory: initialDirectory,
        ))?.path;
    if (result == null) {
      return null;
    } else {
      return p.absolute(result);
    }
  }

  static Future<String> getWorkingDirectory() async {
    if (CurrentPlatform.isAndroid) {
      return (await path_provider.getExternalStorageDirectory())!.path;
    }
    if (CurrentPlatform.isIOS) {
      return (await path_provider.getApplicationDocumentsDirectory()).path;
    }
    if (CurrentPlatform.isWindows) {
      return p.absolute(
        (await path_provider.getApplicationSupportDirectory()).path,
      );
    }
    return (await path_provider.getApplicationSupportDirectory()).path;
  }

  static void createDirectory(String destination) {
    return Directory(destination).createSync(recursive: true);
  }

  static Future<void> createDirectoryAsync(String destination) async {
    await Directory(destination).create(recursive: true);
  }

  static void removeFile(String source) {
    return File(source).deleteSync();
  }

  static Future<void> removeFileAsync(String source) async {
    await File(source).delete(recursive: true);
    return;
  }

  static Future<void> unzipFile(String source, String destination) async {
    final inputStream = InputFileStream(source);
    try {
      final archive = ZipDecoder().decodeStream(inputStream);
      final symbolicLinks = [];
      for (final file in archive.files) {
        if (file.isSymbolicLink) {
          symbolicLinks.add(file);
          continue;
        }
        final filePath = '$destination/${file.name}';
        if (file.isFile) {
          final outputStream = OutputFileStream(filePath);
          file.writeContent(outputStream);
          await outputStream.close();
        } else {
          await Directory(filePath).create(recursive: true);
        }
      }
      for (final entity in symbolicLinks) {
        final link = Link('$destination/${entity.fullPathName}');
        await link.create(entity.symbolicLink!, recursive: true);
      }
    } finally {
      await inputStream.close();
    }
  }

  static void deleteFile({required String source}) {
    File(source).deleteSync();
  }

  static Future<void> deleteFileAsync({required String source}) async {
    await File(source).delete();
  }

  static Future<void> revealFile(String path) async {
    if (CurrentPlatform.isWindows ||
        CurrentPlatform.isMacOS ||
        CurrentPlatform.isLinux) {
      await launchUrl(Uri.file(path), mode: LaunchMode.externalApplication);
    }
    if (CurrentPlatform.isAndroid) {
      final fileUri = Uri.file(path);
      if (await canLaunchUrl(fileUri)) {
        await launchUrl(fileUri, mode: LaunchMode.externalApplication);
      }
    }
    if (CurrentPlatform.isIOS) {
      await launchUrl(
        Uri.file(path).replace(scheme: 'shareddocuments'),
        mode: LaunchMode.externalApplication,
      );
    }
    return;
  }
}
