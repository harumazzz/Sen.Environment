import 'package:flutter/material.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/widget/radio_button.dart';

class BooleanStage extends StatelessWidget {
  const BooleanStage({
    super.key,
    required this.onSend,
  });

  final void Function(String value) onSend;

  bool _canBeRow(BuildContext context) {
    return MediaQuery.of(context).size.width > 1000;
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    final radioButton = RadioButton(
      options: [los.yes, los.no],
      isRowProvider: _canBeRow(context),
    );
    return Row(
      children: [
        Expanded(child: radioButton),
        Tooltip(
          message: los.submit,
          child: IconButton(
            onPressed: () => onSend(radioButton.currentOption),
            icon: const Icon(Symbols.send),
          ),
        ),
      ],
    );
  }
}
