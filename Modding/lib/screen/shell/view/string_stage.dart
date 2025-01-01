import 'package:flutter/material.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:material_symbols_icons/symbols.dart';

class StringStage extends StatelessWidget {
  const StringStage({
    super.key,
    required this.controller,
    required this.onSelect,
    required this.onSend,
  });

  final TextEditingController? controller;

  final VoidCallback onSelect;

  final VoidCallback onSend;

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Row(
      children: [
        Expanded(
          child: TextField(
            controller: controller,
            onSubmitted: (e) => onSend(),
          ),
        ),
        Tooltip(
          message: los.select_option,
          child: IconButton(
            onPressed: onSelect,
            icon: const Icon(Symbols.add_circle_sharp),
          ),
        ),
        Tooltip(
          message: los.submit,
          child: IconButton(
            onPressed: onSend,
            icon: const Icon(Symbols.send),
          ),
        ),
      ],
    );
  }
}
