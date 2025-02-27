import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/bloc/miscellaneous_task_bloc/miscellaneous_task_bloc.dart';
import 'package:sen/extension/context.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/screen/miscellaneous/backup_setting.dart';
import 'package:sen/service/ui_helper.dart';

class MiscellaenousScreen extends StatelessWidget {
  const MiscellaenousScreen({super.key});

  void _onBackup(BuildContext context) {
    Navigator.of(context).push(MaterialPageRoute(
      builder: (context) => const BackupSetting(),
    ));
  }

  Widget _backUpConfiguration(BuildContext context) {
    final los = context.los;
    return Card(
      child: ListTile(
        leading: const Icon(Symbols.backup),
        title: Text(los.backup_configuration),
        subtitle: Text(los.backup_configuration_description),
        onTap: () => _onBackup(context),
      ),
    );
  }

  Widget _installScript(BuildContext context) {
    final los = context.los;
    return BlocBuilder<MiscellaneousTaskBloc, MiscellaneousTaskState>(
      builder: (context, state) {
        return Card(
          child: ListTile(
            leading: const Icon(Symbols.download_2),
            title: Text(los.download_script),
            subtitle: Text(los.download_script_description),
            onTap: () => context.read<MiscellaneousTaskBloc>().add(const DownloadScriptRequested()),
            enabled: Platform.isAndroid,
            trailing: state is DownloadingScript ? const CircularProgressIndicator.adaptive() : null,
          ),
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return BlocProvider<MiscellaneousTaskBloc>(
      create: (context) => MiscellaneousTaskBloc(),
      child: Builder(
        builder: (context) {
          return BlocListener<MiscellaneousTaskBloc, MiscellaneousTaskState>(
            listener: (context, state) async {
              if (state is ScriptDownloaded) {
                await UIHelper.showSimpleDialog(
                  context: context,
                  title: los.done,
                  content: los.script_has_been_downloaded,
                );
              } else if (state is ScriptDownloadFailed) {
                ScaffoldMessenger.of(context).showSnackBar(
                  SnackBar(content: Text(state.error)),
                );
              }
            },
            child: Container(
              margin: const EdgeInsets.symmetric(horizontal: 16.0, vertical: 8.0),
              child: SingleChildScrollView(
                child: Container(
                  constraints: BoxConstraints(
                    minHeight: MediaQuery.of(context).size.height,
                  ),
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      _backUpConfiguration(context),
                      _installScript(context),
                    ],
                  ),
                ),
              ),
            ),
          );
        },
      ),
    );
  }
}
