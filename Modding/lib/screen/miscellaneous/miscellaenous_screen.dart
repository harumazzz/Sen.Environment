import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:page_transition/page_transition.dart';
import 'package:sen/screen/miscellaneous/backup_setting.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

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
            ],
          ),
        ),
      ),
    );
  }
}
