import 'package:flutter_localizations/flutter_localizations.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:dynamic_color/dynamic_color.dart';
import 'package:sen/model/build_distribution.dart';
import 'package:sen/model/theme.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/root_screen.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:flutter/material.dart';

class Application extends ConsumerWidget {
  const Application({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return DynamicColorBuilder(
      builder: (lightDynamic, darkDynamic) => MaterialApp(
        debugShowCheckedModeBanner: false,
        title: BuildDistribution.kApplicationName,
        theme: MaterialDesign.lightTheme.copyWith(
          colorScheme: lightDynamic,
        ),
        darkTheme: MaterialDesign.darkTheme.copyWith(
          colorScheme: darkDynamic,
        ),
        themeMode: ref.watch(settingProvider).themeData,
        home: const RootScreen(title: BuildDistribution.kApplicationName),
        localizationsDelegates: const [
          AppLocalizations.delegate,
          GlobalMaterialLocalizations.delegate,
          GlobalWidgetsLocalizations.delegate,
          GlobalCupertinoLocalizations.delegate,
        ],
        supportedLocales: const [
          Locale('en'),
          Locale('vi'),
          Locale('es'),
          Locale('ru'),
        ],
        locale: Locale(ref.read(settingProvider).locale),
      ),
    );
  }
}
