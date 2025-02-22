import 'package:flutter/widgets.dart';
import 'package:sen/i18n/app_localizations.dart';

extension AppLocalizationsX on BuildContext {
  AppLocalizations get los => AppLocalizations.of(this)!;
}
