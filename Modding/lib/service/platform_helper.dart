import 'dart:io';
import 'package:path/path.dart' as path;

import '../extension/platform.dart';

class PlatformHelper {
  const PlatformHelper._();

  static void revealInExplorer(String filePath) {
    if (CurrentPlatform.isWindows) {
      const executable = 'explorer';
      final arguments = [
        '/select,',
        path.absolute(filePath).replaceAll('/', '\\'),
      ];
      Process.run(executable, arguments);
    } else if (CurrentPlatform.isMacOS) {
      const executable = 'open';
      final arguments = ['-R', path.absolute(filePath)];
      Process.run(executable, arguments);
    } else if (CurrentPlatform.isLinux) {
      const executable = 'xdg-open';
      final arguments = [path.absolute(filePath)];
      Process.run(executable, arguments);
    }
  }
}
