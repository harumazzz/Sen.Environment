import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/screen/map_editor/app/l10n/l10n.dart';
import 'package:sen/screen/map_editor/view/main_page.dart';
import 'package:sen/service/file_helper.dart';

class MapEditor extends StatelessWidget {
  const MapEditor({super.key});

  void _showHelp(
    BuildContext context,
  ) async {
    Future<void> showHelpDialog(String directory) async {
      await showDialog<void>(
        context: context,
        builder: (BuildContext context) {
          return AlertDialog(
            title: const Text('Help'),
            content: SingleChildScrollView(
              child: Column(
                children: [
                  Text('The tool will save the map every 5 minutes automatically in $directory'),
                  const Text('After you save the map, SUI will automatically clean all the files in the directory'),
                ],
              ),
            ),
            actions: <Widget>[
              TextButton(
                onPressed: () {
                  Navigator.of(context).pop();
                },
                child: Text(context.los.okay),
              ),
            ],
          );
        },
      );
    }

    final String directory = await FileHelper.getWorkingDirectory();

    await showHelpDialog(directory);
  }

  @override
  Widget build(BuildContext context) {
    final los = context.los;
    return Scaffold(
      appBar: AppBar(
        forceMaterialTransparency: true,
        title: Text(los.map_editor),
        actions: [
          IconButton(
            icon: const Icon(Symbols.help),
            onPressed: () => _showHelp(context),
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
          return const Background(
            child: Center(
              child: CircularProgressIndicator.adaptive(),
            ),
          );
        },
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
