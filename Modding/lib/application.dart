import 'dart:io';

import 'package:flutter_localizations/flutter_localizations.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:dynamic_color/dynamic_color.dart';
import 'package:sen/model/build_distribution.dart';
import 'package:sen/model/theme.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/animation_viewer/main_screen.dart';
import 'package:sen/screen/javascript_category/javascript_category.dart';
import 'package:sen/screen/level_maker/level_maker.dart';
import 'package:sen/screen/map_editor/map_editor.dart';
import 'package:sen/screen/root_screen.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:flutter/material.dart';
import 'package:sen/screen/shell/shell_screen.dart';
import 'package:windows_taskbar/windows_taskbar.dart';

class Application extends ConsumerStatefulWidget {
  const Application({super.key});

  @override
  ConsumerState<Application> createState() => _ApplicationState();
}

class _ApplicationState extends ConsumerState<Application> {
  late GlobalKey<NavigatorState> _navigatorKey;

  @override
  void initState() {
    _navigatorKey = GlobalKey<NavigatorState>();
    super.initState();
    if (Platform.isWindows) {
      _setupWindowTaskbar();
    }
  }

  void _pushScreen(Widget screen) {
    _navigatorKey.currentState?.push(
      MaterialPageRoute(
        builder: (context) => screen,
      ),
    );
  }

  void _setupWindowTaskbar() {
    WindowsTaskbar.setThumbnailToolbar(
      [
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('assets/icon/terminal.ico'),
          'Shell',
          () => _pushScreen(const ShellScreen(arguments: [])),
        ),
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('assets/icon/js.ico'),
          'JS Category',
          () => _pushScreen(JavaScriptCategory(toolChain: ref.read(settingProvider).toolChain)),
        ),
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('assets/icon/package.ico'),
          'Animation Viewer',
          () => _pushScreen(const AnimationViewer()),
        ),
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('assets/icon/tool.ico'),
          'Level Maker',
          () => _pushScreen(const LevelMaker()),
        ),
        ThumbnailToolbarButton(
          ThumbnailToolbarAssetIcon('assets/icon/map.ico'),
          'Map Editor',
          () => _pushScreen(const MapEditor()),
        ),
      ],
    );
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  @override
  void dispose() {
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return DynamicColorBuilder(
      builder: (lightDynamic, darkDynamic) => MaterialApp(
        navigatorKey: _navigatorKey,
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
