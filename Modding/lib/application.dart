import 'package:dynamic_color/dynamic_color.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:nested/nested.dart';

import 'constant/build_distribution.dart';
import 'constant/localization.dart' as loc;
import 'constant/theme.dart';
import 'cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'cubit/settings_cubit/settings_cubit.dart';
import 'extension/context.dart';
import 'extension/platform.dart';
import 'screen/root_screen.dart';
import 'service/android_helper.dart';
import 'service/ui_helper.dart';

class Application extends StatelessWidget {
  const Application({super.key});

  Future<void> _showPermissionDialog(BuildContext context) async {
    pop() => Navigator.of(context).pop();
    await UIHelper.showFlutterDialog(
      context: context,
      child: UIHelper.buildDialog(
        title: Text(context.los.android_request),
        content: Text(
          context.los.android_storage_access_permission_required,
          overflow: TextOverflow.visible,
        ),
        actions: [
          TextButton(
            onPressed: () async {
              await AndroidHelper.requestStoragePermission();
              pop();
            },
            child: Text(context.los.go_to_settings),
          ),
          TextButton(onPressed: pop, child: Text(context.los.okay)),
        ],
      ),
    );
  }

  void _onListen(BuildContext context, SettingsState state) async {
    Future<void> checkSelfPermission() async =>
        await BlocProvider.of<SettingsCubit>(context).checkAndroidPermission();
    if (CurrentPlatform.isAndroid && !state.requestedPermission) {
      await _showPermissionDialog(context);
      await checkSelfPermission();
    }
  }

  @override
  Widget build(BuildContext context) {
    return MultiBlocProvider(
      providers: <SingleChildWidget>[
        BlocProvider<SettingsCubit>(
          create: (BuildContext context) => SettingsCubit(),
        ),
        BlocProvider<InitialDirectoryCubit>(
          create: (BuildContext context) => InitialDirectoryCubit(),
        ),
        BlocProvider<MapEditorConfigurationCubit>(
          create: (BuildContext context) => MapEditorConfigurationCubit(),
        ),
      ],
      child: BlocBuilder<SettingsCubit, SettingsState>(
        builder: (BuildContext context, SettingsState state) {
          return DynamicColorBuilder(
            builder: (ColorScheme? lightDynamic, ColorScheme? darkDynamic) {
              return MaterialApp(
                debugShowCheckedModeBanner: false,
                title: BuildDistribution.kApplicationName,
                theme: MaterialDesign.lightTheme.copyWith(
                  colorScheme: lightDynamic,
                ),
                darkTheme: MaterialDesign.darkTheme.copyWith(
                  colorScheme: darkDynamic,
                ),
                themeMode: state.themeData,
                home: BlocListener<SettingsCubit, SettingsState>(
                  listenWhen: (previous, current) => previous != current,
                  listener: _onListen,
                  child: const RootScreen(),
                ),
                localizationsDelegates: loc.Localization.localizationDelegates,
                supportedLocales: loc.Localization.supportedLocales,
                locale: Locale(state.locale),
              );
            },
          );
        },
      ),
    );
  }
}
