import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/service/file_helper.dart';
import 'package:sen/i18n/app_localizations.dart';

class LevelMakerConfiguration extends StatefulWidget {
  const LevelMakerConfiguration({
    super.key,
  });

  @override
  State<LevelMakerConfiguration> createState() => _LevelMakerConfigurationState();
}

class _LevelMakerConfigurationState extends State<LevelMakerConfiguration> {
  late TextEditingController _resourceLocationController;

  @override
  void initState() {
    super.initState();
    _resourceLocationController = TextEditingController(
      text: BlocProvider.of<SettingsCubit>(context).state.levelMakerResource,
    );
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  @override
  void dispose() {
    _resourceLocationController.dispose();
    super.dispose();
  }

  Future<void> _onValueChange() async {
    final value = _resourceLocationController.text;
    await BlocProvider.of<SettingsCubit>(context).setLevelMakerResource(value);
  }

  void _onChangeSetting(String? value) async {
    if (value == null) return;
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
    return Padding(
      padding: const EdgeInsets.symmetric(
        horizontal: 12.0,
        vertical: 8.0,
      ),
      child: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.start,
          crossAxisAlignment: CrossAxisAlignment.start,
          spacing: 12.0,
          children: <Widget>[
            Text(
              los.settings,
              style: Theme.of(context).textTheme.titleLarge?.copyWith(fontWeight: FontWeight.bold),
            ),
            Row(
              spacing: 8.0,
              children: [
                Text('${los.resource_location}: '),
                Expanded(
                  child: TextField(
                    controller: _resourceLocationController,
                    onChanged: _onChangeSetting,
                  ),
                ),
                IconButton(
                  onPressed: _onUploadDirectory,
                  icon: Tooltip(
                    message: los.upload_directory,
                    child: const Icon(Symbols.folder),
                  ),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
