import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/bloc/miscellaneous_task_bloc/miscellaneous_task_bloc.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/extension/context.dart';
import 'package:sen/extension/platform.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/screen/miscellaneous/backup_setting.dart';
import 'package:sen/service/ui_helper.dart';

class MiscellaneousScreen extends StatelessWidget {
  const MiscellaneousScreen({super.key});

  void _onBackup(BuildContext context) {
    Navigator.of(context).push(MaterialPageRoute(
      builder: (context) => const BackupSetting(),
    ));
  }

  Widget _backUpConfiguration(BuildContext context) {
    final los = context.los;
    return Card(
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16.0)),
      elevation: 4,
      child: ListTile(
        leading: Icon(Symbols.backup, size: 28, color: Colors.lightBlueAccent.withValues(alpha: 0.8)),
        title: Text(los.backup_configuration, style: Theme.of(context).textTheme.titleMedium),
        subtitle: Text(los.backup_configuration_description, style: Theme.of(context).textTheme.bodyMedium),
        onTap: () => _onBackup(context),
        trailing: const Icon(Symbols.arrow_forward, size: 24.0, color: Colors.grey),
      ),
    );
  }

  Widget _installScript(BuildContext context) {
    final los = context.los;
    isAvailable() => CurrentPlatform.isAndroid
        ? const Icon(Symbols.arrow_downward, size: 24.0, color: Colors.grey)
        : Tooltip(
            message: context.los.not_specified,
            child: const Icon(Symbols.dangerous, size: 24.0, color: Colors.red),
          );
    return BlocBuilder<MiscellaneousTaskBloc, MiscellaneousTaskState>(
      builder: (context, state) {
        return Card(
          shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16.0)),
          elevation: 4.0,
          child: ListTile(
            leading: Icon(Symbols.download_2, size: 28.0, color: Colors.green.withValues(alpha: 0.8)),
            title: Text(los.download_script, style: Theme.of(context).textTheme.titleMedium),
            subtitle: Text(los.download_script_description, style: Theme.of(context).textTheme.bodyMedium),
            onTap: CurrentPlatform.isAndroid
                ? () => BlocProvider.of<MiscellaneousTaskBloc>(context).add(
                      DownloadScriptRequested(
                        settingsCubit: context.read<SettingsCubit>(),
                      ),
                    )
                : null,
            enabled: CurrentPlatform.isAndroid,
            trailing: state is DownloadingScript
                ? const SizedBox(
                    width: 24,
                    height: 24,
                    child: CircularProgressIndicator.adaptive(),
                  )
                : isAvailable(),
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
            child: Padding(
              padding: const EdgeInsets.symmetric(horizontal: 8.0),
              child: Column(
                spacing: 4.0,
                children: [
                  _backUpConfiguration(context),
                  _installScript(context),
                ],
              ),
            ),
          );
        },
      ),
    );
  }
}
