import 'dart:async';
import 'dart:io';

import 'package:dio/dio.dart';
import 'package:sen/model/build_distribution.dart';
import 'package:sen/service/file_service.dart';

class CDNService {
  static Future<String> getCurrentVersionDirectory() async {
    return '${await FileService.getWorkingDirectory()}/CDN${BuildDistribution.version}';
  }

  static Future<void> download({
    required String directory,
    required String link,
    required void Function(int statusCode) onChange,
  }) async {
    try {
      final directoryPath = Directory(directory);
      if (!directoryPath.existsSync()) {
        directoryPath.createSync(recursive: true);
      }
      final fileName = link.split('/').last;
      final filePath = '${directoryPath.path}/$fileName';
      final dio = Dio();
      await dio.download(link, filePath);
      onChange(200);
    } catch (e) {
      onChange(404);
    }
  }
}
