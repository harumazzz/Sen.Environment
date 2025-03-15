import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';

class ReadonlyBoxField extends StatelessWidget {
  const ReadonlyBoxField({super.key, required this.label, required this.value});

  final String label;
  final String value;

  @override
  Widget build(BuildContext context) {
    return TextFormField(
      controller: TextEditingController(text: value),
      decoration: InputDecoration(
        labelText: label,
        labelStyle: const TextStyle(fontWeight: FontWeight.bold),
        floatingLabelBehavior: FloatingLabelBehavior.always,
        contentPadding: const EdgeInsets.fromLTRB(12.0, 4.0, 8.0, 12.0),
        suffixIconConstraints: const BoxConstraints(
          maxHeight: kBottomNavigationBarHeight,
          maxWidth: 24,
        ),
        border: const OutlineInputBorder(),
        suffixIcon: const Padding(
          padding: EdgeInsets.only(top: 8),
          child: SizedBox(width: 30, child: Icon(Symbols.lock, size: 20)),
        ),
      ),
      readOnly: true,
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('label', label));
    properties.add(StringProperty('value', value));
  }
}
