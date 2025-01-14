import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';

class JavaScriptCategoryConfiguration extends StatelessWidget {
  const JavaScriptCategoryConfiguration({
    super.key,
  });

  void _onChangeDialog(
    bool? value,
    BuildContext context,
  ) async {
    if (value == null) return;
    await BlocProvider.of<SettingsCubit>(context).setShowConfirmDialog(value);
  }

  void _onChangeLauncher(
    bool? value,
    BuildContext context,
  ) async {
    if (value == null) return;
    await BlocProvider.of<SettingsCubit>(context).setRunAsLauncher(value);
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return SingleChildScrollView(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        mainAxisAlignment: MainAxisAlignment.start,
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          ListTile(
            contentPadding: EdgeInsets.zero,
            leading: const Icon(Symbols.warning),
            title: Text(
              los.show_confirm_dialog,
              style: Theme.of(context).textTheme.labelLarge,
            ),
            subtitle: Text(
              los.show_confirm_dialog_description,
              style: Theme.of(context).textTheme.labelMedium,
            ),
            trailing: Checkbox(
              value: context.watch<SettingsCubit>().state.jsShowConfirmDialog,
              onChanged: (value) => _onChangeDialog(value, context),
            ),
          ),
          ListTile(
            contentPadding: EdgeInsets.zero,
            leading: const Icon(Symbols.terminal),
            title: Text(
              los.run_as_launcher,
              style: Theme.of(context).textTheme.labelLarge,
            ),
            subtitle: Text(
              los.run_as_launcher_description,
              style: Theme.of(context).textTheme.labelMedium,
            ),
            trailing: Checkbox(
              value: context.watch<SettingsCubit>().state.jsRunAsLauncher,
              onChanged: (value) => _onChangeLauncher(value, context),
            ),
          ),
        ],
      ),
    );
  }
}
