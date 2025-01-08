import 'dart:io';

import 'package:flutter_localizations/flutter_localizations.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:dynamic_color/dynamic_color.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/cubit/javascript_cubit/javascript_cubit.dart';
import 'package:sen/cubit/level_maker_cubit/level_maker_cubit.dart';
import 'package:sen/cubit/map_editor_cubit/map_editor_cubit.dart';
import 'package:sen/model/build_distribution.dart';
import 'package:sen/model/theme.dart';
import 'package:sen/screen/animation_viewer/main_screen.dart';
import 'package:sen/screen/javascript_category/javascript_category.dart';
import 'package:sen/screen/level_maker/level_maker.dart';
import 'package:sen/screen/map_editor/map_editor.dart';
import 'package:sen/screen/root_screen.dart';
import 'package:flutter/material.dart';
import 'package:sen/screen/shell/shell_screen.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:windows_taskbar/windows_taskbar.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

class Application extends StatefulWidget {
  const Application({super.key});

  @override
  State<Application> createState() => _ApplicationState();
}

class _ApplicationState extends State<Application> {
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
          () => _pushScreen(const JavaScriptCategory()),
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
    return MultiBlocProvider(
      providers: [
        BlocProvider<SettingsCubit>(
          create: (context) => SettingsCubit(),
        ),
        BlocProvider<JavascriptCubit>(
          create: (context) => JavascriptCubit(),
        ),
        BlocProvider<MapEditorCubit>(
          create: (context) => MapEditorCubit(),
        ),
        BlocProvider<LevelMakerCubit>(
          create: (context) => LevelMakerCubit(),
        ),
        BlocProvider<InitialDirectoryCubit>(
          create: (context) => InitialDirectoryCubit(),
        ),
      ],
      child: Builder(builder: (context) {
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
            themeMode: BlocProvider.of<SettingsCubit>(context, listen: true).themeData,
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
            locale: Locale(BlocProvider.of<SettingsCubit>(context).state.locale),
          ),
        );
      }),
    );
  }
}
