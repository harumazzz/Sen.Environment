import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../../../cubit/settings_cubit/settings_cubit.dart';
import '../../../service/file_helper.dart';
import '../../../i18n/app_localizations.dart';

class MapEditorConfiguration extends StatefulWidget {
  const MapEditorConfiguration({super.key});

  @override
  State<MapEditorConfiguration> createState() => _MapEditorConfigurationState();
}

class _MapEditorConfigurationState extends State<MapEditorConfiguration> {
  late TextEditingController _resourceLocationController;

  @override
  void initState() {
    super.initState();
    _resourceLocationController = TextEditingController(
      text: BlocProvider.of<SettingsCubit>(context).state.mapEditorResource,
    );
  }

  @override
  void dispose() {
    _resourceLocationController.dispose();
    super.dispose();
  }

  Future<void> _onValueChange() async {
    final value = _resourceLocationController.text;
    context.read<MapEditorConfigurationCubit>().initializeState();
    await BlocProvider.of<SettingsCubit>(context).setMapEditorResource(value);
  }

  void _onChangeSetting(String? value) async {
    if (value == null) {
      return;
    }
    _resourceLocationController.text = value;
    await _onValueChange();
  }

  void _onUploadDirectory() async {
    final result = await FileHelper.uploadDirectory();
    if (result != null) {
      _resourceLocationController.text = result;
      await _onValueChange();
    }
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Column(
      mainAxisSize: MainAxisSize.min,
      crossAxisAlignment: CrossAxisAlignment.start,
      spacing: 10.0,
      children: [
        const SizedBox.shrink(),
        TextField(
          controller: _resourceLocationController,
          decoration: InputDecoration(
            labelText: los.resource_location,
            border: OutlineInputBorder(
              borderRadius: BorderRadius.circular(12.0),
            ),
            suffixIcon: Tooltip(
              message: los.upload_directory,
              child: IconButton(
                onPressed: _onUploadDirectory,
                icon: const Icon(Symbols.folder),
                style: ElevatedButton.styleFrom(
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(12.0),
                  ),
                ),
              ),
            ),
          ),
          onChanged: _onChangeSetting,
        ),
        ElevatedButton(
          onPressed: () {
            context.read<MapEditorConfigurationCubit>().reset();
          },
          child: Padding(
            padding: const EdgeInsets.symmetric(horizontal: 8.0, vertical: 4.0),
            child: Text(los.reload_map_resources),
          ),
        ),
      ],
    );
  }
}
