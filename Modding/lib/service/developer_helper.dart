import 'dart:developer' as developer;

import 'package:flutter/foundation.dart';

class DeveloperHelper {
  const DeveloperHelper._();

  static void debug(String message) {
    if (kDebugMode) {
      developer.log(message);
    }
  }

  static void addDebugDetach({required void Function() builder}) {
    if (kDebugMode) {
      builder();
    }
  }
}
