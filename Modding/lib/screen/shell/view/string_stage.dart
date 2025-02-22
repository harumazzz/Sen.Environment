import 'package:flutter/material.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/screen/shell/view/animated_button.dart';

class StringStage extends StatelessWidget {
  const StringStage({
    super.key,
    required this.controller,
    required this.onSelect,
    required this.onSend,
  });

  final TextEditingController? controller;

  final void Function() onSelect;

  final void Function() onSend;

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
        AnimatedButton(
          message: los.select_option,
          onClick: onSelect,
          child: const Icon(Symbols.add_circle_sharp),
        ),
        AnimatedButton(
          message: los.submit,
          onClick: onSend,
          child: const Icon(Symbols.send),
        ),
      ],
    );
  }
}
