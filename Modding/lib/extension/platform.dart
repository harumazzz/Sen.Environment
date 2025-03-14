import 'dart:io';

extension CurrentPlatform on Platform {
  static bool get isDesktop =>
      Platform.isWindows || Platform.isLinux || Platform.isMacOS;

  static bool get isMobile => Platform.isAndroid || Platform.isIOS;

  static bool get isAndroid => Platform.isAndroid;

  static bool get isIOS => Platform.isIOS;

  static bool get isLinux => Platform.isLinux;

  static bool get isWindows => Platform.isWindows;

  static bool get isMacOS => Platform.isMacOS;

  static String get locale => Platform.localeName.split('_').first;
}
