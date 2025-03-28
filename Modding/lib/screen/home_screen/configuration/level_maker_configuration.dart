import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../../bloc/settings_bloc/settings_bloc.dart';
import '../../../service/file_helper.dart';
import '../../../i18n/app_localizations.dart';

class LevelMakerConfiguration extends StatefulWidget {
  const LevelMakerConfiguration({super.key});

  @override
  State<LevelMakerConfiguration> createState() =>
      _LevelMakerConfigurationState();
}

class _LevelMakerConfigurationState extends State<LevelMakerConfiguration> {
  late TextEditingController _resourceLocationController;

  @override
  void initState() {
    super.initState();
    _resourceLocationController = TextEditingController(
      text: BlocProvider.of<SettingsBloc>(context).state.levelMakerResource,
    );
  }

  @override
  void dispose() {
    _resourceLocationController.dispose();
    super.dispose();
  }

  void _onValueChange() {
    final value = _resourceLocationController.text;
    BlocProvider.of<SettingsBloc>(context).add(SetLevelMakerResource(value));
  }

  void _onChangeSetting(String? value) async {
    if (value == null) {
      return;
    }
    _resourceLocationController.text = value;
    _onValueChange();
  }

  void _onUploadDirectory() async {
    final result = await FileHelper.uploadDirectory();
    if (result != null) {
      _resourceLocationController.text = result;
      _onValueChange();
    }
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Column(
      mainAxisSize: MainAxisSize.min,
      crossAxisAlignment: CrossAxisAlignment.start,
      children: <Widget>[
        TextField(
          minLines: 1,
          maxLines: null,
          keyboardType: TextInputType.multiline,
          controller: _resourceLocationController,
          style: Theme.of(context).textTheme.labelLarge,
          decoration: InputDecoration(
            labelText: los.resource_location,
            border: const OutlineInputBorder(),
            suffixIcon: IconButton(
              onPressed: _onUploadDirectory,
              tooltip: los.upload_directory,
              icon: const Icon(Symbols.folder),
            ),
          ),
          onChanged: _onChangeSetting,
        ),
      ],
    );
  }
}
