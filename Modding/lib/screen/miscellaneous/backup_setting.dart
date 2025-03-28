import 'dart:convert';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:loading_animation_widget/loading_animation_widget.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import '../../bloc/backup_setting_bloc/backup_setting_bloc.dart';
import '../../bloc/initial_directory_bloc/initial_directory_bloc.dart';
import '../../bloc/settings_bloc/settings_bloc.dart';
import '../../extension/context.dart';
import '../../extension/platform.dart';
import '../../i18n/app_localizations.dart';
import '../../service/ui_helper.dart';
import '../../widget/json_viewer.dart';

class BackupSetting extends StatelessWidget {
  const BackupSetting({super.key});

  void _onUpload(BuildContext context) {
    BlocProvider.of<BackupSettingBloc>(context).add(
      UploadConfiguration(
        toolChain: context.read<SettingsBloc>().state.toolChain,
        initialDirectoryBloc: context.read<InitialDirectoryBloc>(),
      ),
    );
  }

  void _onSave(BuildContext context) {
    context.read<BackupSettingBloc>().add(const SaveConfiguration());
  }

  void _onLoad(BuildContext context) {
    context.read<BackupSettingBloc>().add(
      LoadConfiguration(
        toolChain: BlocProvider.of<SettingsBloc>(context).state.toolChain,
      ),
    );
  }

  void _onApply(BuildContext context) {
    BlocProvider.of<BackupSettingBloc>(context).add(
      ApplyConfiguration(
        toolChain: BlocProvider.of<SettingsBloc>(context).state.toolChain,
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;

    return BlocProvider(
      create: (context) => BackupSettingBloc(),
      child: Builder(
        builder: (context) {
          return Scaffold(
            appBar: AppBar(
              forceMaterialTransparency: CurrentPlatform.isDesktop,
              title: Text(los.backup_configuration),
              actions: [
                IconButton(
                  icon: const Icon(Symbols.upload),
                  tooltip: los.upload_configuration,
                  onPressed: () => _onUpload(context),
                ),
                IconButton(
                  icon: const Icon(Symbols.save),
                  tooltip: los.save_configuration,
                  onPressed: () => _onSave(context),
                ),
              ],
            ),
            body: Padding(
              padding: const EdgeInsets.all(8.0),
              child: Column(
                children: [
                  BlocConsumer<BackupSettingBloc, BackupSettingState>(
                    listener: (context, state) async {
                      if (state is BackupError) {
                        await UIHelper.showSimpleDialog(
                          context: context,
                          title: los.error,
                          content: state.message,
                        );
                      }
                    },
                    builder: (context, state) {
                      if (state is ConfigurationLoading) {
                        return const Center(child: CircularProgressIndicator());
                      }
                      if (state is ConfigurationLoaded ||
                          state is ConfigurationUploaded) {
                        return CustomConfigList(config: state.configuration!);
                      }
                      return const CustomLoadingBar();
                    },
                  ),
                  Padding(
                    padding: const EdgeInsets.all(8.0),
                    child: Row(
                      spacing: 6.0,
                      children: [
                        Expanded(
                          child: FilledButton(
                            onPressed: () => _onLoad(context),
                            child: Padding(
                              padding: const EdgeInsets.symmetric(
                                vertical: 8.0,
                              ),
                              child: Text(los.load_configuration),
                            ),
                          ),
                        ),
                        Expanded(
                          child: FilledButton.tonal(
                            onPressed: () => _onApply(context),
                            child: Padding(
                              padding: const EdgeInsets.symmetric(
                                vertical: 8.0,
                              ),
                              child: Text(los.apply_configuration),
                            ),
                          ),
                        ),
                      ],
                    ),
                  ),
                ],
              ),
            ),
          );
        },
      ),
    );
  }
}

class CustomConfigList extends StatelessWidget {
  const CustomConfigList({super.key, required this.config});

  final Map<String, dynamic> config;

  @override
  Widget build(BuildContext context) {
    return Expanded(
      child: ListView.builder(
        padding: const EdgeInsets.symmetric(horizontal: 12.0, vertical: 8.0),
        itemCount: config.length,
        itemBuilder: (context, index) {
          final entry = config.entries.elementAt(index);
          return CustomConfigTile(entry: entry);
        },
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Map<String, dynamic>>('config', config));
  }
}

class CustomLoadingBar extends StatelessWidget {
  const CustomLoadingBar({super.key});

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);

    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 16.0, vertical: 8.0),
      child: Container(
        height: 50.0,
        decoration: BoxDecoration(
          color: theme.colorScheme.surfaceContainerHigh,
          borderRadius: BorderRadius.circular(12.0),
          boxShadow: [
            BoxShadow(
              color: theme.shadowColor.withValues(alpha: 0.1),
              blurRadius: 8,
              spreadRadius: 2,
              offset: const Offset(0, 4),
            ),
          ],
        ),
        child: Row(
          spacing: 12.0,
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            LoadingAnimationWidget.flickr(
              leftDotColor: theme.colorScheme.primary,
              rightDotColor: theme.colorScheme.secondary,
              size: 36,
            ),
            Text(
              '${context.los.waiting_for_user}...',
              textAlign: TextAlign.center,
              style: theme.textTheme.titleMedium?.copyWith(
                fontWeight: FontWeight.bold,
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class CustomConfigTile extends StatelessWidget {
  const CustomConfigTile({super.key, required this.entry});

  final MapEntry<String, dynamic> entry;

  void _onPreviewJson({
    required BuildContext context,
    required String name,
    required String message,
  }) async {
    await UIHelper.showCustomDialog(
      context: context,
      child: JsonViewer(name: name, message: message),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Card(
      margin: const EdgeInsets.symmetric(vertical: 6.0),
      elevation: 2,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(20.0)),
      child: ListTile(
        contentPadding: const EdgeInsets.symmetric(
          horizontal: 16.0,
          vertical: 4.0,
        ),
        leading: Icon(
          Symbols.data_object,
          color: Theme.of(context).colorScheme.primary,
        ),
        title: Text(entry.key, style: Theme.of(context).textTheme.titleMedium),
        trailing: IconButton(
          icon: Icon(
            Symbols.info,
            color: Theme.of(context).colorScheme.onSurfaceVariant,
          ),
          tooltip: context.los.info,
          onPressed: () {
            return _onPreviewJson(
              context: context,
              name: entry.key,
              message: const JsonEncoder.withIndent('\t').convert(entry.value),
            );
          },
        ),
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
        tileColor: Theme.of(context).colorScheme.surfaceContainerLow,
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<MapEntry<String, dynamic>>('entry', entry),
    );
  }
}
