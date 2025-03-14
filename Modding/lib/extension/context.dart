import 'package:flutter/material.dart';
import '../i18n/app_localizations.dart';

extension BuildContextExtension on BuildContext {
  AppLocalizations get los => AppLocalizations.of(this)!;
  bool get isDarkMode => Theme.of(this).brightness == Brightness.dark;
  bool get isDynamicColor =>
      Theme.of(this).colorScheme.surfaceContainerHighest !=
      Theme.of(this).colorScheme.surface;
}
