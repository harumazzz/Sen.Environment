import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/cubit/javascript_cubit/javascript_cubit.dart';

class JavaScriptCategoryConfiguration extends StatelessWidget {
  const JavaScriptCategoryConfiguration({
    super.key,
  });

  void _onChangeDialog(
    bool? value,
    BuildContext context,
  ) async {
    if (value == null) return;
    await BlocProvider.of<JavascriptCubit>(context).setShowConfirmDialog(value);
  }

  void _onChangeLauncher(
    bool? value,
    BuildContext context,
  ) async {
    if (value == null) return;
    await BlocProvider.of<JavascriptCubit>(context).setRunAsLauncher(value);
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Builder(builder: (context) {
      final state = context.watch<JavascriptCubit>().state;
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
                value: state.showConfirmDialog,
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
                value: state.runAsLauncher,
                onChanged: (value) => _onChangeLauncher(value, context),
              ),
            ),
          ],
        ),
      );
    });
  }
}
