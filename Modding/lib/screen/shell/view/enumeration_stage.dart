import 'package:flutter/material.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:material_symbols_icons/symbols.dart';

class EnumerationStage extends StatelessWidget {
  final List<String> enumeration;
  final String value;
  final ValueChanged<String?> onValueChanged;
  final VoidCallback onSend;

  const EnumerationStage({
    super.key,
    required this.enumeration,
    required this.value,
    required this.onValueChanged,
    required this.onSend,
  });

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;

    return Row(
      children: [
        Expanded(
          child: DropdownButton<String>(
            value: value,
            isExpanded: true,
            focusColor: Colors.transparent,
            borderRadius: const BorderRadius.all(Radius.circular(10)),
            underline: Container(),
            items: enumeration
                .map((e) => DropdownMenuItem<String>(
                      value: e,
                      child: Text(e),
                    ))
                .toList(),
            onChanged: onValueChanged,
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
