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
          contentPadding: const EdgeInsets.fromLTRB(0, 4, 0, 12),
          suffixIconConstraints: const BoxConstraints(
              maxHeight: kBottomNavigationBarHeight, maxWidth: 24),
          suffixIcon: const Padding(
            padding: EdgeInsets.only(top: 8),
            child: SizedBox(
              width: 30,
              child: Icon(
                Symbols.lock,
                size: 20,
              ),
            ),
          )),
      readOnly: true,
    );
  }
}
