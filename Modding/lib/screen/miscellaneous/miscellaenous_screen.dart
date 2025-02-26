import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/constant/build_distribution.dart';
import 'package:sen/screen/miscellaneous/backup_setting.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/service/file_helper.dart';
import 'package:dio/dio.dart';

class MiscellaenousScreen extends StatelessWidget {
  const MiscellaenousScreen({
    super.key,
  });

  void _onBackupConfiguration(
    BuildContext context,
  ) async {
    await Navigator.of(context).push(
      MaterialPageRoute(
        builder: (context) => const BackupSetting(),
      ),
    );
  }

  void _onDownloadScript(
    AppLocalizations los,
    BuildContext context,
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

    void setToolChain({
      required String destination,
    }) async {
      await BlocProvider.of<SettingsCubit>(context).setToolChain(destination);
    }

    void setValidate() async {
      await BlocProvider.of<SettingsCubit>(context).setIsValid(true);
    }

    final destination = '${await FileHelper.getWorkingDirectory()}/CDN ${BuildDistribution.version}';
    if (!FileHelper.isDirectory(destination)) {
      FileHelper.createDirectory(destination);
    }
    final source = '$destination/Script.zip';
    final dio = Dio();
    await dio.download(
      'https://github.com/harumazzz/Sen.Environment/releases/download/script/Script.zip',
      source,
    );
    await FileHelper.unzipFile(source, '$destination/Script');

    FileHelper.removeFile(source);
    showSuccessDialog();
    setToolChain(destination: destination);
    setValidate();
  }

  @override
  Widget build(BuildContext context) {
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
                  onTap: () => _onDownloadScript(los, context),
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
