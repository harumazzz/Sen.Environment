import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../bloc/miscellaneous_task_bloc/miscellaneous_task_bloc.dart';
import '../../cubit/settings_cubit/settings_cubit.dart';
import '../../extension/context.dart';
import '../../extension/platform.dart';
import '../../i18n/app_localizations.dart';
import 'backup_setting.dart';
import '../../service/ui_helper.dart';

class MiscellaneousScreen extends StatelessWidget {
  const MiscellaneousScreen({super.key});

  void _onBackup(BuildContext context) {
    Navigator.of(
      context,
    ).push(MaterialPageRoute(builder: (context) => const BackupSetting()));
  }

  Widget _backUpConfiguration(BuildContext context) {
    final los = context.los;
    return Card(
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16.0)),
      elevation: 4,
      child: ListTile(
        leading: Icon(
          Symbols.backup,
          size: 28,
          color: Colors.lightBlueAccent.withValues(alpha: 0.8),
        ),
        title: Text(
          los.backup_configuration,
          style: Theme.of(context).textTheme.titleMedium,
        ),
        subtitle: Text(
          los.backup_configuration_description,
          style: Theme.of(context).textTheme.bodyMedium,
        ),
        onTap: () => _onBackup(context),
        trailing: const Icon(
          Symbols.arrow_forward,
          size: 24.0,
          color: Colors.grey,
        ),
      ),
    );
  }

  Widget _installScript(BuildContext context) {
    final los = context.los;
    isAvailable() =>
        CurrentPlatform.isAndroid
            ? const Icon(Symbols.arrow_downward, size: 24.0, color: Colors.grey)
            : Tooltip(
              message: context.los.not_specified,
              child: const Icon(
                Symbols.dangerous,
                size: 24.0,
                color: Colors.red,
              ),
            );
    return BlocBuilder<MiscellaneousTaskBloc, MiscellaneousTaskState>(
      builder: (context, state) {
        return Card(
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(16.0),
          ),
          elevation: 4.0,
          child: ListTile(
            leading: Icon(
              Symbols.download_2,
              size: 28.0,
              color: Colors.green.withValues(alpha: 0.8),
            ),
            title: Text(
              los.download_script,
              style: Theme.of(context).textTheme.titleMedium,
            ),
            subtitle: Text(
              los.download_script_description,
              style: Theme.of(context).textTheme.bodyMedium,
            ),
            onTap:
                CurrentPlatform.isAndroid
                    ? () => BlocProvider.of<MiscellaneousTaskBloc>(context).add(
                      DownloadScriptRequested(
                        settingsCubit: context.read<SettingsCubit>(),
                      ),
                    )
                    : null,
            enabled: CurrentPlatform.isAndroid,
            trailing:
                state is DownloadingScript
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
                  SnackBar(
                    content: Text(
                      state.error,
                      softWrap: true,
                      overflow: TextOverflow.visible,
                    ),
                    width: UIHelper.ofDesktop(builder: () => 400.0),
                    behavior: SnackBarBehavior.floating,
                    duration: const Duration(milliseconds: 1500),
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(10.0),
                    ),
                  ),
                );
              }
            },
            child:
                (() {
                  if (CurrentPlatform.isDesktop) {
                    return _buildDesktopLayout(context);
                  }
                  return _buildMobileLayout(context);
                })(),
          );
        },
      ),
    );
  }

  Widget _buildMobileLayout(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: Column(
        spacing: 4.0,
        children: [_backUpConfiguration(context), _installScript(context)],
      ),
    );
  }

  Widget _buildDesktopLayout(BuildContext context) {
    return SafeArea(
      child: ListView(
        padding: const EdgeInsets.all(8.0),
        children: [
          Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            mainAxisSize: MainAxisSize.min,
            children: [_buildBackupCard(context), _buildDownloadCard(context)],
          ),
        ],
      ),
    );
  }

  Widget _buildBackupCard(BuildContext context) {
    final los = context.los;
    return Card(
      elevation: 4,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16.0)),
      child: Padding(
        padding: const EdgeInsets.symmetric(vertical: 12.0, horizontal: 20.0),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            Row(
              spacing: 10.0,
              children: [
                Icon(
                  Symbols.backup,
                  size: 28,
                  color: Colors.lightBlueAccent.withValues(alpha: 0.8),
                ),
                Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  spacing: 4.0,
                  children: [
                    Text(
                      los.backup_configuration,
                      style: Theme.of(context).textTheme.titleMedium,
                    ),
                    Text(
                      los.backup_configuration_description,
                      style: Theme.of(context).textTheme.bodyMedium,
                    ),
                  ],
                ),
              ],
            ),
            FilledButton.icon(
              onPressed: () => _onBackup(context),
              label: Text(los.backup_configuration),
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildDownloadCard(BuildContext context) {
    final los = context.los;
    return Card(
      elevation: 4,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16.0)),
      child: Padding(
        padding: const EdgeInsets.symmetric(vertical: 12.0, horizontal: 20.0),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            Row(
              spacing: 12.0,
              children: [
                Icon(
                  Symbols.download_2,
                  size: 28.0,
                  color: Colors.green.withValues(alpha: 0.8),
                ),
                Column(
                  spacing: 4.0,
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Text(
                      los.download_script,
                      style: Theme.of(context).textTheme.titleMedium,
                    ),
                    Text(
                      los.download_script_description,
                      style: Theme.of(context).textTheme.bodyMedium,
                    ),
                  ],
                ),
              ],
            ),
            FilledButton.icon(
              onPressed: null,
              label: Text(los.download_script),
            ),
          ],
        ),
      ),
    );
  }
}
