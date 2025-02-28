import 'package:flutter/material.dart';
import 'package:flutter_localizations/flutter_localizations.dart';
import 'package:sen/i18n/app_localizations.dart';

class Localization {
  static const supportedLocales = [
    Locale('en'),
    Locale('vi'),
    Locale('es'),
    Locale('ru'),
  ];

  static const localizationDelegates = [
    AppLocalizations.delegate,
    GlobalMaterialLocalizations.delegate,
    GlobalWidgetsLocalizations.delegate,
    GlobalCupertinoLocalizations.delegate,
  ];
}
