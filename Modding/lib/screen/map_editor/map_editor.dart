import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../../cubit/settings_cubit/settings_cubit.dart';
import '../../extension/context.dart';
import '../../widget/background.dart';
import '../../widget/collapsible_floating_button.dart';
import 'bloc/init_bloc/init_bloc.dart';
import 'loading_screen.dart';
import 'view/main_page.dart';
import '../../service/ui_helper.dart';

class MapEditor extends StatelessWidget {
  const MapEditor({super.key});

  Future<bool?> _confirmExit(BuildContext context) async {
    final los = context.los;
    return await showDialog(
      context: context,
      builder: (context) {
        return AlertDialog(
          title: Text(los.exit),
          content: Text(los.confirm_exit),
          actions: [
            TextButton(
              child: Text(los.cancel),
              onPressed: () => Navigator.of(context).pop(false),
            ),
            TextButton(
              child: Text(los.yes),
              onPressed: () => Navigator.of(context).pop(true),
            ),
          ],
        );
      },
    );
  }

  Widget? _buildLeading(BuildContext context) {
    return UIHelper.ofMobile(
      builder: () {
        return IconButton(
          tooltip: context.los.back,
          onPressed: () async {
            final status =
                context.read<MapEditorConfigurationCubit>().state.status;
            if (status == AppConfigurationStatus.success) {
              if (await _confirmExit(context) ?? false) {
                if (context.mounted) {
                  Navigator.of(context).pop();
                }
              }
            } else {
              Navigator.of(context).pop();
            }
          },
          icon: const Icon(Symbols.arrow_back),
        );
      },
    );
  }

  Widget _buildFloatingActionButton({
    required BuildContext context,
    required VoidCallback screenshot,
  }) {
    return CollapsibleFloatingActionButton(
      items: [
        CollapsibleFloatingActionButtonItem(
          icon: Symbols.screenshot,
          tooltip: context.los.take_screenshot,
          onPressed: screenshot,
        ),
      ],
    );
  }

  Future<void> _onScreenshot({
    required BuildContext context,
    required Future<Uint8List?> Function()? takeShoot,
  }) async {
    if (takeShoot != null) {
      final imageBytes = await takeShoot();
      if (imageBytes != null && context.mounted) {
        await UIHelper.showScreenshotDialog(context, imageBytes);
      }
    }
  }

  PreferredSizeWidget? _buildAppBar(
    BuildContext context,
    MapEditorConfigurationState state,
    Future<Uint8List?> Function()? takeShoot,
  ) {
    return UIHelper.ofMobile(
      builder: () {
        return AppBar(
          title: Text(
            context.los.map_editor,
            style: const TextStyle(fontWeight: FontWeight.bold),
          ),
          leading: _buildLeading(context),
          actions: [
            IconButton(
              onPressed:
                  () async => await _onScreenshot(
                    context: context,
                    takeShoot: takeShoot,
                  ),
              icon: const Icon(Symbols.screenshot),
            ),
          ],
        );
      },
    );
  }

  Widget _buildFailedState(
    BuildContext context,
    MapEditorConfigurationState state,
  ) {
    return Background(
      child: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          spacing: 8.0,
          children: [
            Text(
              '${context.los.loading_error}: ${state.errorSnapShot}',
              style: Theme.of(context).textTheme.bodyLarge,
            ),
            ElevatedButton(
              onPressed: () {
                context.read<MapEditorConfigurationCubit>().reset();
              },
              child: Text(context.los.reload_map_resources),
            ),
          ],
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    var takeShoot = null as Future<Uint8List?> Function()?;
    return BlocProvider<InitBloc>(
      create: (_) => InitBloc(),
      child: BlocListener<InitBloc, InitState>(
        listener: (context, state) {
          takeShoot = state.takeShoot;
        },
        child: BlocBuilder<
          MapEditorConfigurationCubit,
          MapEditorConfigurationState
        >(
          buildWhen: (prev, state) => prev.status != state.status,
          builder: (context, state) {
            WidgetsBinding.instance.addPostFrameCallback((_) async {
              if (state.status == AppConfigurationStatus.initial) {
                final resourceLocation =
                    context.read<SettingsCubit>().state.mapEditorResource;
                spawn() async => await context
                    .read<MapEditorConfigurationCubit>()
                    .load(context.los, resourceLocation);
                await Future.delayed(Duration.zero);
                await spawn();
              }
            });
            return Scaffold(
              appBar: _buildAppBar(context, state, takeShoot),
              body: Builder(
                builder: (context) {
                  if (state.status == AppConfigurationStatus.success) {
                    return Background(
                      child: MainPage(
                        mapEditorConfigurationCubit:
                            context.read<MapEditorConfigurationCubit>(),
                      ),
                    );
                  } else if (state.status == AppConfigurationStatus.failed) {
                    return _buildFailedState(context, state);
                  }
                  return const LoadingScreen();
                },
              ),
              floatingActionButton: UIHelper.ofDesktop(
                builder: () {
                  return _buildFloatingActionButton(
                    context: context,
                    screenshot:
                        () async => await _onScreenshot(
                          context: context,
                          takeShoot: takeShoot,
                        ),
                  );
                },
              ),
            );
          },
        ),
      ),
    );
  }
}
