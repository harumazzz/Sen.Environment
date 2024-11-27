import 'dart:async';
import 'dart:io';

import 'package:sen/model/build_distribution.dart';
import 'package:sen/service/android_service.dart';
import 'package:sen/service/file_service.dart';

class CDNService {
  static Future<String> getCurrentVersionDirectory() async {
    return '${await FileService.getWorkingDirectory()}/CDN${BuildDistribution.version}';
  }

  static Future<void> download({
    required String destination,
    required String url,
  }) async {
    final directoryPath = Directory(destination);
    if (!directoryPath.existsSync()) {
      directoryPath.createSync(recursive: true);
    }
    final fileName = url.split('/').last;
    final state = await AndroidService.downloadFile(
      description: "Downloading file...",
      destination: destination,
      fileName: fileName,
      url: url,
    );
    if (state != null && !state) {
      throw Exception("File cannot be downloaded: $fileName");
    }
  }

  static Future<void> unzipFile({
    required String source,
    required String destination,
  }) async {
    final state = await AndroidService.unzipFile(
      source: source,
      destination: destination,
    );
    if (state != null && !state) {
      throw Exception("File cannot be unzip: $source");
    }
    return;
  }
}
