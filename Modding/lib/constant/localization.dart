import 'package:flutter/material.dart';
import 'package:flutter_localizations/flutter_localizations.dart';
import '../i18n/app_localizations.dart';

class Localization {
  static final supportedLocales = locales.map(Locale.new);

  static const locales = ['en', 'vi', 'es', 'ru'];

  static const localizationDelegates = [
    AppLocalizations.delegate,
    GlobalMaterialLocalizations.delegate,
    GlobalWidgetsLocalizations.delegate,
    GlobalCupertinoLocalizations.delegate,
  ];
}
