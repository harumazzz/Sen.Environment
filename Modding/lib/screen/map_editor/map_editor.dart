import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/extension/l10n.dart';
import 'package:sen/screen/map_editor/bloc/init_bloc/init_bloc.dart';
import 'package:sen/screen/map_editor/view/main_page.dart';
import 'package:sen/service/ui_helper.dart';

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

  @override
  Widget build(BuildContext context) {
    final los = context.los;
    Future<Uint8List?> Function()? takeShoot;
    return BlocProvider<InitBloc>(
      create: (_) => InitBloc(),
      child: BlocListener<InitBloc, InitState>(
        listener: (context, state) {
          takeShoot = state.takeShoot;
        },
        child: Scaffold(
          appBar: AppBar(
            forceMaterialTransparency: true,
            title: Text(los.map_editor),
            leading: Tooltip(
                message: los.back,
                child: IconButton(
                    onPressed: () async {
                      final status = context.read<MapEditorConfigurationCubit>().state.status;
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
                    icon: const Icon(Icons.arrow_back))),
            actions: [
              IconButton(
                onPressed: () async {
                  if (takeShoot != null) {
                    final imageBytes = await takeShoot!();
                    if (imageBytes != null && context.mounted) {
                      await UIHelper.showScreenshotDialog(context, imageBytes);
                    }
                  }
                },
                icon: const Icon(Symbols.screenshot),
              ),
            ],
          ),
          body: BlocBuilder<MapEditorConfigurationCubit, MapEditorConfigurationState>(
            buildWhen: (prev, state) => prev.status != state.status,
            builder: (context, state) {
              final status = state.status;
              if (status == AppConfigurationStatus.initial) {
                final resourceLocation = context.read<SettingsCubit>().state.mapEditorResource;
                context.read<MapEditorConfigurationCubit>().load(context.los, resourceLocation);
              }
              if (status == AppConfigurationStatus.success) {
                return Background(
                  child: MainPage(mapEditorConfigurationCubit: context.read<MapEditorConfigurationCubit>()),
                );
              } else if (status == AppConfigurationStatus.failed) {
                return Background(
                    child: Center(
                  child: Text(
                    '${los.loading_error}: ${state.errorSnapShot}',
                    style: Theme.of(context).textTheme.bodyLarge,
                  ),
                ));
              }
              return Background(
                child: Center(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.center,
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
            colors: [
              topColor,
              bottomColor,
            ],
          ),
        ),
        child: child,
      ),
    );
  }
}
