import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/provider/js_provider.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class JavaScriptCategoryConfiguration extends ConsumerWidget {
  const JavaScriptCategoryConfiguration({
    super.key,
  });

  void _onChange(bool? value, WidgetRef ref) {
    if (value == null) return;
    ref.read(jsProvider.notifier).setshowConfirmDialog(value);
  }

  @override
  Widget build(BuildContext context, WidgetRef ref) {
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
            trailing: Checkbox(
              value: ref.watch(jsProvider).showConfirmDialog,
              onChanged: (value) => _onChange(value, ref),
            ),
          ),
        ],
      ),
    );
  }
}
