import 'package:dynamic_color/dynamic_color.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'package:sen/constant/build_distribution.dart';
import 'package:sen/constant/theme.dart';
import 'package:sen/screen/root_screen.dart';
import 'package:flutter/material.dart';
import 'package:sen/constant/localization.dart' as loc;
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

class Application extends StatelessWidget {
  const Application({super.key});

  @override
  Widget build(BuildContext context) {
    return MultiBlocProvider(
      providers: [
        BlocProvider<SettingsCubit>(create: (context) => SettingsCubit()),
        BlocProvider<InitialDirectoryCubit>(
          create: (context) => InitialDirectoryCubit(),
        ),
        BlocProvider<MapEditorConfigurationCubit>(
          create: (context) => MapEditorConfigurationCubit(),
        ),
      ],
      child: BlocBuilder<SettingsCubit, SettingsState>(
        builder: (context, state) {
          return DynamicColorBuilder(
            builder: (lightDynamic, darkDynamic) {
              final bucket = PageStorageBucket();
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
