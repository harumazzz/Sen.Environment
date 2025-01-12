import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';

class ShellConfiguration extends StatelessWidget {
  const ShellConfiguration({
    super.key,
  });

  void _onChange(
    bool? value,
    BuildContext context,
  ) async {
    if (value == null) return;
    await BlocProvider.of<SettingsCubit>(context).setShellLaunchImmediately(value);
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Builder(builder: (context) {
      final state = context.watch<SettingsCubit>().state;
      return SingleChildScrollView(
        child: Column(
          mainAxisSize: MainAxisSize.min,
          mainAxisAlignment: MainAxisAlignment.start,
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            ListTile(
              contentPadding: EdgeInsets.zero,
              leading: const Icon(Symbols.launch),
              title: Text(
                los.run_immediately,
                style: Theme.of(context).textTheme.labelLarge,
              ),
              subtitle: Text(
                los.run_immediately_description,
                style: Theme.of(context).textTheme.labelMedium,
              ),
              trailing: Checkbox(
                value: state.shellLaunchImmediately,
                onChanged: (value) => _onChange(value, context),
              ),
            ),
          ],
        ),
      );
    });
  }
}
