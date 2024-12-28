import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:page_transition/page_transition.dart';
import 'package:sen/model/build_distribution.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/miscellaneous/backup_setting.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/service/file_service.dart';
import 'package:dio/dio.dart';

class MiscellaenousScreen extends ConsumerWidget {
  const MiscellaenousScreen({
    super.key,
  });

  void _onBackupConfiguration(
    BuildContext context,
  ) async {
    await Navigator.of(context).push(
      PageTransition(
        type: PageTransitionType.rightToLeft,
        child: const BackupSetting(),
      ),
    );
  }

  void _onDownloadScript(
    AppLocalizations los,
    BuildContext context,
    WidgetRef ref,
  ) async {
    void showSuccessDialog() async {
      final los = AppLocalizations.of(context)!;
      await showDialog(
        context: context,
        builder: (context) {
          return AlertDialog(
            title: Text(los.done),
            content: Text(los.script_has_been_downloaded),
            actions: [
              TextButton(
                onPressed: () => Navigator.of(context).pop(),
                child: Text(los.ok),
              ),
            ],
          );
        },
      );
    }

    final destination = '${await FileService.getWorkingDirectory()}/CDN ${BuildDistribution.version}';
    if (!FileService.isDirectory(destination)) {
      FileService.createDirectory(destination);
    }
    final source = '$destination/Script.zip';
    final dio = Dio();
    await dio.download(
      'https://github.com/harumazzz/Sen.Environment/releases/download/script/Script.zip',
      source,
    );
    await FileService.unzipFile(source, '$destination/Script');

    FileService.removeFile(source);
    await ref.read(settingProvider.notifier).setToolChain(destination);
    await ref.read(settingProvider.notifier).setIsValid(true);
    showSuccessDialog();
  }

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final los = AppLocalizations.of(context)!;
    return Container(
      margin: const EdgeInsets.symmetric(
        horizontal: 16.0,
        vertical: 8.0,
      ),
      child: SingleChildScrollView(
        child: Container(
          constraints: BoxConstraints(
            minHeight: MediaQuery.of(context).size.height,
          ),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.start,
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Card(
                child: ListTile(
                  leading: const Icon(Symbols.backup),
                  title: Text(los.backup_configuration),
                  subtitle: Text(los.backup_configuration_description),
                  onTap: () => _onBackupConfiguration(context),
                ),
              ),
              Card(
                child: ListTile(
                  leading: const Icon(Symbols.download_2),
                  title: Text(los.download_script),
                  subtitle: Text(los.download_script_description),
                  onTap: () => _onDownloadScript(los, context, ref),
                  enabled: Platform.isAndroid,
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
