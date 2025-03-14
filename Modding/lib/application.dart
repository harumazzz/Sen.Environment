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
import 'screen/root_screen.dart';

class Application extends StatelessWidget {
  const Application({super.key});

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
              final PageStorageBucket bucket = PageStorageBucket();
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
                home: RootScreen(bucket: bucket),
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
