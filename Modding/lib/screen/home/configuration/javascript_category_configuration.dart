import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/cubit/javascript_cubit/javascript_cubit.dart';

@immutable
class JavaScriptCategoryConfiguration extends StatelessWidget {
  const JavaScriptCategoryConfiguration({
    super.key,
  });

  void _onChange(
    bool? value,
    BuildContext context,
  ) async {
    if (value == null) return;
    await BlocProvider.of<JavascriptCubit>(context).setshowConfirmDialog(value);
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
            leading: const Icon(Symbols.warning),
            title: Text(los.show_confirm_dialog),
            trailing: BlocBuilder<JavascriptCubit, JavascriptState>(
              builder: (context, state) {
                return Checkbox(
                  value: state.showConfirmDialog,
                  onChanged: (value) => _onChange(value, context),
                );
              },
            ),
          ),
        ],
      ),
    );
  }
}
