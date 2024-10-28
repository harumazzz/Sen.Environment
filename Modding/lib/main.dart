import 'dart:io';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:sen/model/build_distribution.dart';
import 'package:sen/model/theme.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/root_screen.dart';
import 'package:sen/service/android_service.dart';
import 'package:sen/service/notification_service.dart';
import 'package:window_manager/window_manager.dart';
import 'package:flutter_localizations/flutter_localizations.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:dynamic_color/dynamic_color.dart';

Future<void> main(
  List<String> arguments,
) async {
  WidgetsFlutterBinding.ensureInitialized();
  if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
    await windowManager.setTitleBarStyle(TitleBarStyle.normal);
    await WindowManager.instance.ensureInitialized();
    await windowManager.setMinimumSize(const Size(1280, 720));
    await windowManager.center();
    await windowManager.waitUntilReadyToShow();
    await windowManager.show();
    await windowManager.focus();
  }
  await NotificationService.initialize();
  if (Platform.isAndroid) {
    AndroidService.initialize();
  }
  runApp(
    ProviderScope(
      child: const Application(),
    ),
  );
}

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
