import 'dart:io';

import 'package:flutter_localizations/flutter_localizations.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:dynamic_color/dynamic_color.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'package:sen/constant/build_distribution.dart';
import 'package:sen/constant/theme.dart';
import 'package:sen/screen/animation_viewer/animation_viewer.dart';
import 'package:sen/screen/javascript_category/javascript_category.dart';
import 'package:sen/screen/level_maker/level_maker.dart';
import 'package:sen/screen/map_editor/map_editor.dart';
import 'package:sen/screen/root_screen.dart';
import 'package:flutter/material.dart';
import 'package:sen/screen/shell/shell_screen.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:windows_taskbar/windows_taskbar.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

class Application extends StatelessWidget {
  const Application({super.key});

  @override
  Widget build(BuildContext context) {
    if (Platform.isWindows) {
      return const _WindowsApplication();
    } else {
      return const _MainApplication();
    }
  }
}

class _WindowsApplication extends StatefulWidget {
  const _WindowsApplication();

  @override
  State<_WindowsApplication> createState() => _WindowsApplicationState();
}

class _WindowsApplicationState extends State<_WindowsApplication> {
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
          () => _pushScreen(const ShellScreen()),
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
  Widget build(BuildContext context) {
    return _MainApplication(
      navigatorKey: _navigatorKey,
    );
  }
}

class _MainApplication extends StatelessWidget {
  const _MainApplication({
    this.navigatorKey,
  });

  final GlobalKey<NavigatorState>? navigatorKey;

  @override
  Widget build(BuildContext context) {
    return MultiBlocProvider(
      providers: [
        BlocProvider<SettingsCubit>(
          create: (context) => SettingsCubit(),
        ),
        BlocProvider<InitialDirectoryCubit>(
          create: (context) => InitialDirectoryCubit(),
        ),
        if (Platform.isWindows || Platform.isLinux || Platform.isMacOS)
          BlocProvider<MapEditorConfigurationCubit>(
            create: (context) => MapEditorConfigurationCubit(),
          ),
      ],
      child: Builder(
        builder: (context) {
          final settings = BlocProvider.of<SettingsCubit>(context, listen: true);
          return DynamicColorBuilder(
            builder: (lightDynamic, darkDynamic) => MaterialApp(
              navigatorKey: navigatorKey,
              debugShowCheckedModeBanner: false,
              title: BuildDistribution.kApplicationName,
              theme: MaterialDesign.lightTheme.copyWith(colorScheme: lightDynamic),
              darkTheme: MaterialDesign.darkTheme.copyWith(colorScheme: darkDynamic),
              themeMode: settings.themeData,
              home: const RootScreen(),
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
              locale: Locale(settings.state.locale),
            ),
          );
        },
      ),
    );
  }
}
