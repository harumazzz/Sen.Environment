import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/service/file_helper.dart';
import 'package:path/path.dart' as p;
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/widget/hotkey.dart';

class BackupSetting extends StatefulWidget {
  const BackupSetting({
    super.key,
  });

  @override
  State<BackupSetting> createState() => _BackupSettingState();
}

class _BackupSettingState extends State<BackupSetting> {
  Map<String, dynamic>? _configuration;

  @override
  void initState() {
    super.initState();
  }

  @override
  void dispose() {
    super.dispose();
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  void _loadConfig({
    required String source,
  }) {
    final sourceFiles = FileHelper.readDirectory(source: source, recursive: false)
        .where((e) => RegExp(r'(.+)\.json$', caseSensitive: false).hasMatch(e))
        .toList();
    _configuration = {};
    for (final e in sourceFiles) {
      _configuration![p.basenameWithoutExtension(e)] = jsonDecode(FileHelper.readFile(source: e));
    }
  }

  void _loadDumpedConfiguration({
    required String source,
  }) {
    _configuration = jsonDecode(FileHelper.readFile(source: source));
    for (var e in _configuration!.entries) {
      _configuration![e.key] = e.value;
    }
  }

  void _onLoadConfiguration({
    required String toolChain,
  }) {
    final los = AppLocalizations.of(context)!;
    try {
      final source = '$toolChain/Script/Executor/Configuration';
      _loadConfig(source: source);
      setState(() {});
      _onSuccess(message: los.configuration_has_been_loaded);
    } catch (e, s) {
      _onErrorDialog(e.toString(), s);
    }
  }

  void _setWorkingDirectory(String source) {
    BlocProvider.of<InitialDirectoryCubit>(context).setDirectoryOfFile(source: source);
  }

  void _onUploadConfiguration() async {
    final los = AppLocalizations.of(context)!;
    final source = await FileHelper.uploadFile(
      initialDirectory: BlocProvider.of<InitialDirectoryCubit>(context).state.initialDirectory,
    );
    if (source == null) {
      return;
    }
    try {
      _setWorkingDirectory(source);
      _loadDumpedConfiguration(source: source);
      setState(() {});
      _onSuccess(message: los.configuration_has_been_loaded);
    } catch (e, s) {
      _onErrorDialog(e.toString(), s);
    }
  }

  void _onErrorDialog(
    String message,
    StackTrace stack,
  ) async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.invalid_request),
        content: Text(message),
        actions: [
          TextButton(
            onPressed: () async {
              Navigator.of(context).pop();
              await showDialog(
                context: context,
                builder: (context) => AlertDialog(
                  title: Text(message),
                  content: Text(stack.toString()),
                  actions: [
                    TextButton(
                      onPressed: () {
                        Navigator.of(context).pop();
                      },
                      child: Text(los.okay),
                    ),
                  ],
                ),
              );
            },
            child: Text(los.detail),
          ),
          TextButton(
            onPressed: () {
              Navigator.of(context).pop();
            },
            child: Text(los.okay),
          ),
        ],
      ),
    );
  }

  void _onSuccess({
    required String message,
  }) async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.done),
        content: Text(message),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop();
            },
            child: Text(los.okay),
          ),
        ],
      ),
    );
  }

  void _onPreviewJson({
    required String file,
    required String message,
  }) async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(file),
        content: Text(message),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop();
            },
            child: Text(los.okay),
          ),
        ],
      ),
    );
  }

  void _applyChange(
    Map<String, dynamic> current,
    Map<String, dynamic> config,
  ) {
    for (var key in config.keys) {
      if (current.containsKey(key)) {
        current[key] = config[key];
      }
    }
  }

  void _onApplyChange({
    required String toolChain,
  }) {
    final configuration = '$toolChain/Script/Executor/Configuration';
    for (var e in _configuration!.entries) {
      if (e.value is Map<String, dynamic> && (e.value as Map<String, dynamic>).isNotEmpty) {
        final destination = '$configuration/${e.key}.json';
        final current = FileHelper.readJson(source: destination);
        _applyChange(current, e.value);
        FileHelper.writeJson(source: destination, data: current);
      }
    }
  }

  void _onConfirm({
    required String toolChain,
  }) async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.confirmation),
        content: Text(los.confirm_apply_configuration),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop();
              _onApplyChange(toolChain: toolChain);
              _onSuccess(message: los.successfully_applied_your_configuration);
            },
            child: Text(los.yes),
          ),
          TextButton(
            onPressed: () {
              Navigator.of(context).pop();
            },
            child: Text(los.no),
          ),
        ],
      ),
    );
  }

  void _onApplyConfiguration({
    required String toolChain,
  }) {
    if (_configuration == null) return;
    try {
      _onConfirm(toolChain: toolChain);
    } catch (e, s) {
      _onErrorDialog(e.toString(), s);
    }
  }

  void _saveConfiguration() async {
    if (_configuration == null) {}
    final destination = await FileHelper.saveFile(
      suggestedName: 'configuration.json',
    );
    if (destination == null) return;
    FileHelper.writeJson(source: destination, data: _configuration);
  }

  Widget _buildExpandableList() {
    final los = AppLocalizations.of(context)!;
    return Column(
      children: _configuration!.entries
          .map(
            (e) => Card(
              child: ListTile(
                leading: const Icon(Symbols.data_object),
                title: Text(e.key),
                trailing: Tooltip(
                  message: los.info,
                  child: IconButton(
                    icon: const Icon(Symbols.info),
                    onPressed: () => _onPreviewJson(
                      file: e.key,
                      message: const JsonEncoder.withIndent('\t').convert(e.value),
                    ),
                  ),
                ),
              ),
            ),
          )
          .toList(),
    );
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return HotkeyBuilder(
      child: Scaffold(
        appBar: AppBar(
          title: Text(los.backup_configuration),
          actions: [
            Tooltip(
              message: los.upload_configuration,
              child: IconButton(
                onPressed: _onUploadConfiguration,
                icon: const Icon(Symbols.upload),
              ),
            ),
            Tooltip(
              message: los.save_configuration,
              child: IconButton(
                onPressed: _saveConfiguration,
                icon: const Icon(Symbols.save),
              ),
            ),
          ],
        ),
        body: SingleChildScrollView(
          child: Padding(
            padding: const EdgeInsets.all(8.0),
            child: Column(
              spacing: 15.0,
              children: [
                Card(
                  child: ListTile(
                    leading: const Icon(Symbols.package_2),
                    title: Text(los.toolchain),
                    subtitle: Text(
                      BlocProvider.of<SettingsCubit>(context).state.toolChain,
                    ),
                  ),
                ),
                Row(
                  children: [
                    Expanded(
                      child: ElevatedButton(
                        onPressed: () => _onLoadConfiguration(
                          toolChain: BlocProvider.of<SettingsCubit>(context).state.toolChain,
                        ),
                        child: Padding(
                          padding: const EdgeInsets.symmetric(vertical: 12.0),
                          child: Text(los.load_configuration),
                        ),
                      ),
                    ),
                    Expanded(
                      child: ElevatedButton(
                        onPressed: () => _onApplyConfiguration(
                          toolChain: BlocProvider.of<SettingsCubit>(context).state.toolChain,
                        ),
                        child: Padding(
                          padding: const EdgeInsets.symmetric(vertical: 12.0),
                          child: Text(los.apply_configuration),
                        ),
                      ),
                    ),
                  ],
                ),
                _configuration == null ? const SizedBox.shrink() : _buildExpandableList(),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
