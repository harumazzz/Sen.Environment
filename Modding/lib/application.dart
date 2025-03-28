import 'package:dynamic_color/dynamic_color.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/gestures.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:nested/nested.dart';

import 'bloc/initial_directory_bloc/initial_directory_bloc.dart';
import 'bloc/miscellaneous_task_bloc/miscellaneous_task_bloc.dart';
import 'constant/build_distribution.dart';
import 'constant/theme.dart';
import 'cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'bloc/settings_bloc/settings_bloc.dart';
import 'extension/context.dart';
import 'extension/platform.dart';
import 'i18n/app_localizations.dart';
import 'screen/root_screen.dart';
import 'service/android_helper.dart';
import 'service/file_helper.dart';
import 'service/ui_helper.dart';

class Application extends StatelessWidget {
  const Application({super.key, required this.navigatorKey});

  final GlobalKey<NavigatorState> navigatorKey;

  Future<void> _showPermissionDialog(BuildContext context) async {
    pop() => Navigator.of(context).pop();
    await UIHelper.showFlutterDialog(
      context: context,
      child: UIHelper.buildDialog(
        title: Text(context.los.android_request),
        content: Text(
          context.los.android_storage_access_permission_required,
          overflow: TextOverflow.visible,
          style: Theme.of(
            context,
          ).textTheme.titleMedium?.copyWith(fontWeight: FontWeight.w600),
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

  Future<void> _showSetupToolChain(BuildContext context) async {
    await UIHelper.showCustomDialog(
      context: context,
      barrierDismissible: false,
      child: BlocProvider(
        create: (context) => MiscellaneousTaskBloc(),
        child: const _SetupToolChainDialog(),
      ),
    );
  }

  Future<void> _onListen(BuildContext context, SettingsState state) async {
    void checkSelfPermission() => BlocProvider.of<SettingsBloc>(
      context,
    ).add(const CheckAndroidPermission());
    Future<void> setupToolChain() async => await _showSetupToolChain(context);
    checkSelfPermission();
    if (CurrentPlatform.isAndroid && !state.requestedPermission) {
      await _showPermissionDialog(context);
      checkSelfPermission();
    }
    if (await _checkDefaultPermission() && state.toolChain.isEmpty) {
      await setupToolChain();
    }
  }

  Future<bool> _checkDefaultPermission() async {
    return CurrentPlatform.isAndroid
        ? await AndroidHelper.checkStoragePermission()
        : true;
  }

  @override
  Widget build(BuildContext context) {
    return MultiBlocProvider(
      providers: <SingleChildWidget>[
        BlocProvider<SettingsBloc>(
          create:
              (BuildContext context) =>
                  SettingsBloc()..add(const LoadSettings()),
        ),
        BlocProvider<InitialDirectoryBloc>(
          create: (BuildContext context) => InitialDirectoryBloc(),
        ),
        BlocProvider<MapEditorConfigurationCubit>(
          create: (BuildContext context) => MapEditorConfigurationCubit(),
        ),
      ],
      child: BlocBuilder<SettingsBloc, SettingsState>(
        builder: (BuildContext context, SettingsState state) {
          return DynamicColorBuilder(
            builder: (ColorScheme? lightDynamic, ColorScheme? darkDynamic) {
              return MaterialApp(
                navigatorKey: navigatorKey,
                debugShowCheckedModeBanner: false,
                title: BuildDistribution.kApplicationName,
                theme: MaterialDesign.lightTheme.copyWith(
                  colorScheme: lightDynamic,
                ),
                darkTheme: MaterialDesign.darkTheme.copyWith(
                  colorScheme: darkDynamic,
                ),
                themeMode: state.themeData,
                themeAnimationStyle: AnimationStyle(
                  duration: const Duration(milliseconds: 250),
                  curve: Curves.easeInOut,
                ),
                home: BlocListener<SettingsBloc, SettingsState>(
                  listenWhen: (previous, current) => previous != current,
                  listener: _onListen,
                  child: const RootScreen(),
                ),
                scrollBehavior: ScrollConfiguration.of(context).copyWith(
                  multitouchDragStrategy: MultitouchDragStrategy.sumAllPointers,
                ),
                localizationsDelegates: AppLocalizations.localizationsDelegates,
                supportedLocales: AppLocalizations.supportedLocales,
                locale: Locale(state.locale),
              );
            },
          );
        },
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<GlobalKey<NavigatorState>>(
        'navigatorKey',
        navigatorKey,
      ),
    );
  }
}

class _SetupToolChainDialog extends StatefulWidget {
  const _SetupToolChainDialog();

  @override
  State<_SetupToolChainDialog> createState() => __SetupToolChainDialogState();
}

class __SetupToolChainDialogState extends State<_SetupToolChainDialog> {
  late TextEditingController _controller;

  @override
  void initState() {
    super.initState();
    _controller = TextEditingController();
    _controller.addListener(() async {
      final bloc = context.read<SettingsBloc>();
      Future<bool> getIsValid() async => await _onCheckValidator(context);
      bloc.add(SetToolChain(_controller.text));
      bloc.add(SetIsValid(isValid: await getIsValid()));
    });
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  Future<bool> _onCheckValidator(BuildContext context) async {
    if (BlocProvider.of<SettingsBloc>(context).state.toolChain.isNotEmpty) {
      final state =
          _existKernel(
            BlocProvider.of<SettingsBloc>(context).state.toolChain,
          ) &&
          _existScript(BlocProvider.of<SettingsBloc>(context).state.toolChain);
      BlocProvider.of<SettingsBloc>(context).add(SetIsValid(isValid: state));
      return state;
    }
    return false;
  }

  bool _existScript(String path) {
    return FileHelper.isFile('$path/Script/main.js');
  }

  bool _existKernel(String path) {
    if (CurrentPlatform.isAndroid) {
      return true;
    }
    if (CurrentPlatform.isWindows) {
      return FileHelper.isFile('$path/Kernel.dll');
    }
    if (CurrentPlatform.isLinux) {
      return FileHelper.isFile('$path/libKernel.so');
    }
    if (CurrentPlatform.isIOS || CurrentPlatform.isMacOS) {
      return FileHelper.isFile('$path/libKernel.dylib');
    }
    return false;
  }

  @override
  Widget build(BuildContext context) {
    return BlocProvider(
      create: (context) => MiscellaneousTaskBloc(),
      child: BlocConsumer<MiscellaneousTaskBloc, MiscellaneousTaskState>(
        buildWhen: (previous, current) => previous != current,
        listenWhen: (previous, current) => previous != current,
        listener: (context, state) async {
          if (state is ScriptDownloaded) {
            Navigator.of(context).pop();
            Navigator.of(context).pop();
            await UIHelper.showSimpleDialog(
              context: context,
              title: context.los.congratulation,
              content: context.los.congratulation_description,
            );
          }
        },
        builder: (context, state) {
          return Card(
            child: Padding(
              padding: const EdgeInsets.all(24.0),
              child: Column(
                mainAxisSize: MainAxisSize.min,
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  const CustomTitle(),
                  const SizedBox(height: 12),
                  const Expanded(child: CustomDescription()),
                  const SizedBox(height: 12),
                  if (CurrentPlatform.isDesktop)
                    CustomToolchainInput(
                      controller: _controller,
                      onCheck: _onCheckValidator,
                    ),
                  const SizedBox(height: 16),
                  if (CurrentPlatform.isMobile && state is! DownloadingScript)
                    const CustomDownloadButton(),
                  if (state is DownloadingScript) const CustomLoading(),
                  if (CurrentPlatform.isDesktop) const CustomSubmitButton(),
                ],
              ),
            ),
          );
        },
      ),
    );
  }
}

class CustomTitle extends StatelessWidget {
  const CustomTitle({super.key});

  @override
  Widget build(BuildContext context) {
    return Text(
      context.los.setup_toolchain,
      style: Theme.of(
        context,
      ).textTheme.titleLarge?.copyWith(fontWeight: FontWeight.bold),
      maxLines: 4,
      overflow: TextOverflow.ellipsis,
    );
  }
}

class AdditionalModules extends StatelessWidget {
  const AdditionalModules({super.key});

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        ...[
          context.los.map_editor_setup,
          context.los.level_maker_setup,
          context.los.other_modules_setup,
          context.los.setup_ending,
        ].map(
          (text) => Padding(
            padding: const EdgeInsets.only(top: 4),
            child: Text(
              text,
              style: Theme.of(context).textTheme.titleSmall,
              maxLines: 4,
              overflow: TextOverflow.ellipsis,
            ),
          ),
        ),
      ],
    );
  }
}

class MobileInstructions extends StatelessWidget {
  const MobileInstructions({super.key});

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      spacing: 4.0,
      children: [
        Text(
          context.los.for_mobile,
          style: Theme.of(context).textTheme.titleSmall,
        ),
        Row(
          spacing: 8.0,
          children: [
            Icon(
              Symbols.circle,
              size: 8,
              color: Theme.of(context).colorScheme.primary,
            ),
            Expanded(
              child: Text(
                context.los.setup_toolchain_mobile,
                style: Theme.of(context).textTheme.titleSmall,
                maxLines: 4,
                overflow: TextOverflow.ellipsis,
              ),
            ),
          ],
        ),
      ],
    );
  }
}

class CustomBulletPoint extends StatelessWidget {
  const CustomBulletPoint({super.key, required this.text});

  final String text;

  @override
  Widget build(BuildContext context) {
    return Row(
      spacing: 8.0,
      children: [
        Icon(
          Symbols.circle,
          size: 8,
          color: Theme.of(context).colorScheme.primary,
        ),
        Expanded(
          child: Text(
            text,
            style: Theme.of(context).textTheme.titleSmall,
            maxLines: 4,
            overflow: TextOverflow.ellipsis,
          ),
        ),
      ],
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('text', text));
  }
}

class DesktopInstructions extends StatelessWidget {
  const DesktopInstructions({super.key});

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          context.los.for_desktop,
          style: Theme.of(context).textTheme.titleSmall,
        ),
        const SizedBox(height: 4),
        ...[
          context.los.setup_toolchain_desktop,
          context.los.setup_toolchain_desktop_01,
          context.los.setup_toolchain_desktop_02,
        ].map((e) => CustomBulletPoint(text: e)),
      ],
    );
  }
}

class CustomToolchainInput extends StatelessWidget {
  const CustomToolchainInput({
    super.key,
    required this.controller,
    required this.onCheck,
  });

  final TextEditingController controller;

  final Future<bool> Function(BuildContext context) onCheck;

  @override
  Widget build(BuildContext context) {
    throw Row(
      children: [
        Expanded(
          child: TextField(
            controller: controller,
            decoration: InputDecoration(
              labelText: context.los.toolchain,
              border: const OutlineInputBorder(),
            ),
          ),
        ),
        IconButton(
          tooltip: context.los.upload_directory,
          onPressed: () async {
            Future<bool> verify() async => await onCheck(context);
            Future<void> setToolchain(String directory) async {
              final bloc = context.read<SettingsBloc>();
              bloc.add(SetToolChain(directory));
              bloc.add(SetIsValid(isValid: await verify()));
            }

            final directory = await FileHelper.uploadDirectory();
            if (directory != null) {
              controller.text = directory;
              await setToolchain(directory);
            }
          },
          icon: const Icon(Symbols.drive_folder_upload),
        ),
      ],
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<TextEditingController>('controller', controller),
    );
    properties.add(
      ObjectFlagProperty<Future<bool> Function(BuildContext context)>.has(
        'onCheck',
        onCheck,
      ),
    );
  }
}

class CustomDownloadButton extends StatelessWidget {
  const CustomDownloadButton({super.key});

  void _onPress(BuildContext context) {
    context.read<MiscellaneousTaskBloc>().add(
      DownloadScriptRequested(settingsBloc: context.read<SettingsBloc>()),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.end,
      children: [
        ElevatedButton(
          onPressed: () => _onPress(context),
          child: Text(context.los.download_script),
        ),
      ],
    );
  }
}

class CustomLoading extends StatelessWidget {
  const CustomLoading({super.key});

  @override
  Widget build(BuildContext context) {
    return const Row(
      mainAxisAlignment: MainAxisAlignment.end,
      children: [CircularProgressIndicator()],
    );
  }
}

class CustomSubmitButton extends StatelessWidget {
  const CustomSubmitButton({super.key});

  Future<void> _onPress(BuildContext context) async {
    if (BlocProvider.of<SettingsBloc>(context).state.isValid) {
      Navigator.of(context).pop();
      await UIHelper.showSimpleDialog(
        context: context,
        title: context.los.congratulation,
        content: context.los.congratulation_description,
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.end,
      children: [
        ElevatedButton(
          onPressed: () async => await _onPress(context),
          child: Text(context.los.submit),
        ),
      ],
    );
  }
}

class CustomDescription extends StatelessWidget {
  const CustomDescription({super.key});

  @override
  Widget build(BuildContext context) {
    return ListView(
      shrinkWrap: true,
      children: [
        Text(
          context.los.setup_toolchain_description,
          style: Theme.of(context).textTheme.titleSmall,
        ),
        const SizedBox(height: 8),
        if (CurrentPlatform.isMobile) const MobileInstructions(),
        if (CurrentPlatform.isDesktop) const DesktopInstructions(),
        const AdditionalModules(),
      ],
    );
  }
}
