import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../../cubit/settings_cubit/settings_cubit.dart';
import '../../extension/context.dart';
import '../../extension/platform.dart';
import '../../widget/collapsible_floating_button.dart';
import 'bloc/init_bloc/init_bloc.dart';
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
    return CurrentPlatform.isDesktop
        ? null
        : Tooltip(
          message: context.los.back,
          child: IconButton(
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
          ),
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

  @override
  Widget build(BuildContext context) {
    final los = context.los;
    var takeShoot = null as Future<Uint8List?> Function()?;
    return BlocProvider<InitBloc>(
      create: (_) => InitBloc(),
      child: BlocListener<InitBloc, InitState>(
        listener: (context, state) {
          takeShoot = state.takeShoot;
        },
        child: Scaffold(
          appBar: UIHelper.appBarOr(
            AppBar(
              title: Text(los.map_editor),
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
            ),
          ),
          body: BlocBuilder<
            MapEditorConfigurationCubit,
            MapEditorConfigurationState
          >(
            buildWhen: (prev, state) => prev.status != state.status,
            builder: (context, state) {
              final status = state.status;
              if (status == AppConfigurationStatus.initial) {
                final resourceLocation =
                    context.read<SettingsCubit>().state.mapEditorResource;
                WidgetsBinding.instance.addPostFrameCallback((_) async {
                  spawn() async => await context
                      .read<MapEditorConfigurationCubit>()
                      .load(context.los, resourceLocation);
                  await Future.delayed(Duration.zero);
                  await spawn();
                });
              }
              if (status == AppConfigurationStatus.success) {
                return Background(
                  child: MainPage(
                    mapEditorConfigurationCubit:
                        context.read<MapEditorConfigurationCubit>(),
                  ),
                );
              } else if (status == AppConfigurationStatus.failed) {
                return Background(
                  child: Center(
                    child: Column(
                      mainAxisAlignment: MainAxisAlignment.center,
                      spacing: 8.0,
                      children: [
                        Text(
                          '${los.loading_error}: ${state.errorSnapShot}',
                          style: Theme.of(context).textTheme.bodyLarge,
                        ),
                        ElevatedButton(
                          onPressed: () {
                            context.read<MapEditorConfigurationCubit>().reset();
                          },
                          child: Text(los.reload_map_resources),
                        ),
                      ],
                    ),
                  ),
                );
              }
              return Background(
                child: Center(
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    spacing: 16.0,
                    children: [
                      const CircularProgressIndicator.adaptive(),
                      Text(
                        los.loading_map_resources,
                        style: Theme.of(context).textTheme.labelMedium,
                      ),
                    ],
                  ),
                ),
              );
            },
          ),
          floatingActionButton: UIHelper.widgetOr(
            _buildFloatingActionButton(
              context: context,
              screenshot:
                  () async => await _onScreenshot(
                    context: context,
                    takeShoot: takeShoot,
                  ),
            ),
          ),
        ),
      ),
    );
  }
}

class Background extends StatelessWidget {
  const Background({super.key, required this.child});

  final Widget child;

  @override
  Widget build(BuildContext context) {
    final topColor = Theme.of(context).canvasColor;
    final bottomColor = Theme.of(context).colorScheme.surfaceContainerLow;
    return SizedBox.expand(
      child: DecoratedBox(
        decoration: BoxDecoration(
          gradient: LinearGradient(
            begin: Alignment.topCenter,
            end: Alignment.bottomCenter,
            colors: [topColor, bottomColor],
          ),
        ),
        child: child,
      ),
    );
  }
}
