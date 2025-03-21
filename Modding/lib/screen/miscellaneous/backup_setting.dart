import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:loading_animation_widget/loading_animation_widget.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import '../../bloc/backup_setting_bloc/backup_setting_bloc.dart';
import '../../cubit/initial_directory_cubit/initial_directory_cubit.dart';
import '../../cubit/settings_cubit/settings_cubit.dart';
import '../../extension/context.dart';
import '../../extension/platform.dart';
import '../../i18n/app_localizations.dart';
import '../../service/ui_helper.dart';
import '../../widget/json_viewer.dart';

class BackupSetting extends StatelessWidget {
  const BackupSetting({super.key});

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
                  onPressed: () {
                    BlocProvider.of<BackupSettingBloc>(context).add(
                      UploadConfiguration(
                        toolChain:
                            context.read<SettingsCubit>().state.toolChain,
                        initialDirectoryCubit:
                            context.read<InitialDirectoryCubit>(),
                      ),
                    );
                  },
                ),
                IconButton(
                  icon: const Icon(Symbols.save),
                  tooltip: los.save_configuration,
                  onPressed: () {
                    context.read<BackupSettingBloc>().add(
                      const SaveConfiguration(),
                    );
                  },
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
                        return _buildConfigList(context, state.configuration!);
                      }
                      return _buildLoadingBar(context: context);
                    },
                  ),
                  Padding(
                    padding: const EdgeInsets.all(8.0),
                    child: Row(
                      spacing: 6.0,
                      children: [
                        Expanded(
                          child: FilledButton(
                            onPressed: () {
                              context.read<BackupSettingBloc>().add(
                                LoadConfiguration(
                                  toolChain:
                                      context
                                          .read<SettingsCubit>()
                                          .state
                                          .toolChain,
                                ),
                              );
                            },
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
                            onPressed: () {
                              context.read<BackupSettingBloc>().add(
                                ApplyConfiguration(
                                  toolChain:
                                      BlocProvider.of<SettingsCubit>(
                                        context,
                                      ).state.toolChain,
                                ),
                              );
                            },
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

  Widget _buildLoadingBar({required BuildContext context}) {
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

  Widget _buildConfigList(BuildContext context, Map<String, dynamic> config) {
    return Expanded(
      child: ListView.builder(
        padding: const EdgeInsets.symmetric(horizontal: 12.0, vertical: 8.0),
        itemCount: config.length,
        itemBuilder: (context, index) {
          final entry = config.entries.elementAt(index);
          return _buildConfigTile(context, entry);
        },
      ),
    );
  }

  Widget _buildConfigTile(
    BuildContext context,
    MapEntry<String, dynamic> entry,
  ) {
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
          onPressed:
              () => _onPreviewJson(
                context: context,
                name: entry.key,
                message: const JsonEncoder.withIndent(
                  '\t',
                ).convert(entry.value),
              ),
        ),
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
        tileColor: Theme.of(context).colorScheme.surfaceContainerLow,
      ),
    );
  }
}
