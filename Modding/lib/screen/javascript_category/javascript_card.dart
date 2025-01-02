import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:page_transition/page_transition.dart';
import 'package:sen/model/script.dart';
import 'package:sen/provider/js_provider.dart';
import 'package:sen/screen/shell/shell_screen.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class JavaScriptCard extends ConsumerWidget {
  const JavaScriptCard({
    super.key,
    required this.item,
    required this.toolChain,
  });

  final Holder item;

  final String toolChain;

  Color? _color(BuildContext context) {
    return Theme.of(context).brightness == Brightness.dark ? Colors.yellowAccent.withValues(alpha: 0.8) : null;
  }

  List<String> _makeArguments() {
    return [
      '-method',
      'js.evaluate',
      '-source',
      item.path.replaceFirst(
        '.',
        '$toolChain/Script/Helper',
      ),
    ];
  }

  void _onTap(BuildContext context, WidgetRef ref) async {
    if (context.mounted) {
      await _onConfirm(
        ref,
        context,
        () async => await Navigator.of(context).push(
          PageTransition(
            duration: const Duration(milliseconds: 300),
            type: PageTransitionType.rightToLeft,
            child: ShellScreen(
              arguments: _makeArguments(),
            ),
          ),
        ),
      );
    }
  }

  Future<void> _onConfirm(
    WidgetRef ref,
    BuildContext context,
    Future<dynamic> Function() function,
  ) async {
    final los = AppLocalizations.of(context)!;
    if (ref.read(jsProvider).showConfirmDialog) {
      await showDialog(
        context: context,
        builder: (context) => AlertDialog(
          title: Text(los.confirmation),
          content: Text(los.do_you_wish_to_run(item.name)),
          actions: [
            TextButton(
              onPressed: () => Navigator.of(context).pop(),
              child: Text(los.cancel),
            ),
            TextButton(
              onPressed: () async {
                Navigator.of(context).pop();
                await function();
              },
              child: Text(los.okay),
            ),
          ],
        ),
      );
      return;
    }
    return await function();
  }

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final los = AppLocalizations.of(context)!;
    return Card(
      child: ListTile(
        leading: Icon(Symbols.javascript, size: 28, color: _color(context)),
        title: Text(
          item.name,
          style: Theme.of(context).textTheme.labelMedium?.copyWith(fontWeight: FontWeight.bold),
        ),
        subtitle: Text(
          item.description,
          style: Theme.of(context).textTheme.labelSmall,
        ),
        onTap: () => _onTap(context, ref),
        trailing: Tooltip(
          message: los.js_execute,
          child: const Icon(Symbols.arrow_forward),
        ),
      ),
    );
  }
}
