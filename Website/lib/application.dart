import 'package:flutter/material.dart';
import 'package:flutter_localizations/flutter_localizations.dart';
import 'package:website/i18n/app_localizations.dart';
import 'package:website/screen/root_page.dart';
import 'dart:ui' as ui;

class Application extends StatelessWidget {
  const Application({super.key});

  Locale _getInitialLocale() {
    final String browserLocale =
        ui.PlatformDispatcher.instance.locale.toLanguageTag().split('-').first;
    for (final locale in AppLocalizations.supportedLocales) {
      if (locale.languageCode == browserLocale) {
        return locale;
      }
    }
    return const Locale('en');
  }

  @override
  Widget build(BuildContext context) {
    return Builder(
      builder: (context) {
        return MaterialApp(
          title: 'Sen: Download page',
          theme: ThemeData(
            pageTransitionsTheme: PageTransitionsTheme(
              builders:
                  Map<TargetPlatform, PageTransitionsBuilder>.fromIterable(
                    TargetPlatform.values,
                    value: (_) => const FadeForwardsPageTransitionsBuilder(),
                  ),
            ),
            fontFamily: 'GoogleSans',
            colorScheme: ColorScheme.fromSeed(
              seedColor: Colors.deepPurple,
              brightness: Brightness.light,
            ),
            useMaterial3: true,
          ),
          debugShowCheckedModeBanner: false,
          darkTheme: ThemeData(
            pageTransitionsTheme: PageTransitionsTheme(
              builders:
                  Map<TargetPlatform, PageTransitionsBuilder>.fromIterable(
                    TargetPlatform.values,
                    value: (_) => const FadeForwardsPageTransitionsBuilder(),
                  ),
            ),
            fontFamily: 'GoogleSans',
            brightness: Brightness.dark,
            colorScheme: ColorScheme.fromSeed(
              seedColor: Colors.deepPurpleAccent,
              brightness: Brightness.dark,
            ).copyWith(surface: Colors.black, onSurface: Colors.white),
            scaffoldBackgroundColor: Colors.black,
            useMaterial3: true,
            dialogTheme: DialogTheme(backgroundColor: Colors.grey.shade900),
          ),
          themeMode: ThemeMode.system,
          localizationsDelegates: const [
            AppLocalizations.delegate,
            GlobalMaterialLocalizations.delegate,
            GlobalWidgetsLocalizations.delegate,
            GlobalCupertinoLocalizations.delegate,
          ],
          supportedLocales: AppLocalizations.supportedLocales,
          locale: _getInitialLocale(),
          home: const RootPage(),
        );
      },
    );
  }
}
