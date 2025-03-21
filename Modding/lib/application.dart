import 'package:dynamic_color/dynamic_color.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/gestures.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:nested/nested.dart';

import 'bloc/miscellaneous_task_bloc/miscellaneous_task_bloc.dart';
import 'constant/build_distribution.dart';
import 'constant/theme.dart';
import 'cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'cubit/settings_cubit/settings_cubit.dart';
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

  void _onListen(BuildContext context, SettingsState state) async {
    Future<void> checkSelfPermission() async =>
        await BlocProvider.of<SettingsCubit>(context).checkAndroidPermission();
    Future<void> setupToolChain() async => await _showSetupToolChain(context);
    if (CurrentPlatform.isAndroid && !state.requestedPermission) {
      await _showPermissionDialog(context);
      await checkSelfPermission();
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
                home: BlocListener<SettingsCubit, SettingsState>(
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
      final cubit = context.read<SettingsCubit>();
      Future<bool> getIsValid() async => await _onCheckValidator(context);
      await cubit.setToolChain(_controller.text);
      await cubit.setIsValid(isValid: await getIsValid());
    });
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  Widget _buildTitle() {
    return Text(
      context.los.setup_toolchain,
      style: Theme.of(
        context,
      ).textTheme.titleLarge?.copyWith(fontWeight: FontWeight.bold),
      maxLines: 4,
      overflow: TextOverflow.ellipsis,
    );
  }

  Widget _buildDescription() {
    return ListView(
      shrinkWrap: true,
      children: [
        Text(
          context.los.setup_toolchain_description,
          style: Theme.of(context).textTheme.titleSmall,
        ),
        const SizedBox(height: 8),
        if (CurrentPlatform.isMobile) _buildMobileInstructions(),
        if (CurrentPlatform.isDesktop) _buildDesktopInstructions(),
        _buildAdditionalModules(),
      ],
    );
  }

  Widget _buildMobileInstructions() {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          context.los.for_mobile,
          style: Theme.of(context).textTheme.titleSmall,
        ),
        const SizedBox(height: 4),
        Row(
          children: [
            Icon(
              Symbols.circle,
              size: 8,
              color: Theme.of(context).colorScheme.primary,
            ),
            const SizedBox(width: 8),
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

  Widget _buildDesktopInstructions() {
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
        ].map(_buildBulletPoint),
      ],
    );
  }

  Widget _buildBulletPoint(String text) {
    return Row(
      children: [
        Icon(
          Symbols.circle,
          size: 8,
          color: Theme.of(context).colorScheme.primary,
        ),
        const SizedBox(width: 8),
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

  Widget _buildAdditionalModules() {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children:
          [
                context.los.map_editor_setup,
                context.los.level_maker_setup,
                context.los.other_modules_setup,
                context.los.setup_ending,
              ]
              .map(
                (text) => Padding(
                  padding: const EdgeInsets.only(top: 4),
                  child: Text(
                    text,
                    style: Theme.of(context).textTheme.titleSmall,
                    maxLines: 4,
                    overflow: TextOverflow.ellipsis,
                  ),
                ),
              )
              .toList(),
    );
  }

  Widget _buildToolchainInput() {
    return Row(
      children: [
        Expanded(
          child: TextField(
            controller: _controller,
            decoration: InputDecoration(
              labelText: context.los.toolchain,
              border: const OutlineInputBorder(),
            ),
          ),
        ),
        IconButton(
          tooltip: context.los.upload_directory,
          onPressed: () async {
            Future<bool> verify() async => await _onCheckValidator(context);
            Future<void> setToolchain(String directory) async {
              final cubit = context.read<SettingsCubit>();
              await cubit.setToolChain(directory);
              await cubit.setIsValid(isValid: await verify());
            }

            final directory = await FileHelper.uploadDirectory();
            if (directory != null) {
              _controller.text = directory;
              await setToolchain(directory);
            }
          },
          icon: const Icon(Symbols.drive_folder_upload),
        ),
      ],
    );
  }

  Widget _buildDownloadButton() {
    return Row(
      mainAxisAlignment: MainAxisAlignment.end,
      children: [
        ElevatedButton(
          onPressed: () {
            context.read<MiscellaneousTaskBloc>().add(
              DownloadScriptRequested(
                settingsCubit: context.read<SettingsCubit>(),
              ),
            );
          },
          child: Text(context.los.download_script),
        ),
      ],
    );
  }

  Widget _buildLoading() {
    return const Row(
      mainAxisAlignment: MainAxisAlignment.end,
      children: [CircularProgressIndicator()],
    );
  }

  Widget _buildSubmitButton() {
    return Row(
      mainAxisAlignment: MainAxisAlignment.end,
      children: [
        ElevatedButton(
          onPressed: () async {
            if (BlocProvider.of<SettingsCubit>(context).state.isValid) {
              Navigator.of(context).pop();
              await UIHelper.showSimpleDialog(
                context: context,
                title: context.los.congratulation,
                content: context.los.congratulation_description,
              );
            }
          },
          child: Text(context.los.submit),
        ),
      ],
    );
  }

  Future<bool> _onCheckValidator(BuildContext context) async {
    if (BlocProvider.of<SettingsCubit>(context).state.toolChain.isNotEmpty) {
      final state =
          _existKernel(
            BlocProvider.of<SettingsCubit>(context).state.toolChain,
          ) &&
          _existScript(BlocProvider.of<SettingsCubit>(context).state.toolChain);
      await BlocProvider.of<SettingsCubit>(context).setIsValid(isValid: state);
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
                  _buildTitle(),
                  const SizedBox(height: 12),
                  Expanded(child: _buildDescription()),
                  const SizedBox(height: 12),
                  if (CurrentPlatform.isDesktop) _buildToolchainInput(),
                  const SizedBox(height: 16),
                  if (CurrentPlatform.isMobile && state is! DownloadingScript)
                    _buildDownloadButton(),
                  if (state is DownloadingScript) _buildLoading(),
                  if (CurrentPlatform.isDesktop) _buildSubmitButton(),
                ],
              ),
            ),
          );
        },
      ),
    );
  }
}
