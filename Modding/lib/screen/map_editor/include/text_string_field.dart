import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';

class TextStringField extends StatelessWidget {
  const TextStringField({
    super.key,
    required this.label,
    required this.value,
    required this.onFieldSubmitted,
    this.autovalidateMode,
    this.validator,
  });

  final String label;

  final String? value;

  final void Function(String) onFieldSubmitted;

  final AutovalidateMode? autovalidateMode;

  final String? Function(String?)? validator;

  @override
  Widget build(BuildContext context) {
    final textController = TextEditingController(text: value);
    return TextFormField(
      controller: textController,
      decoration: InputDecoration(
        labelText: label,
        labelStyle: const TextStyle(fontWeight: FontWeight.bold),
        contentPadding: const EdgeInsets.fromLTRB(12.0, 4.0, 8.0, 12.0),
        floatingLabelBehavior: FloatingLabelBehavior.always,
        border: const OutlineInputBorder(),
      ),
      autovalidateMode: autovalidateMode,
      validator: validator,
      onFieldSubmitted: onFieldSubmitted,
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('label', label));
    properties.add(StringProperty('value', value));
    properties.add(
      ObjectFlagProperty<void Function(String p1)>.has(
        'onFieldSubmitted',
        onFieldSubmitted,
      ),
    );
    properties.add(
      EnumProperty<AutovalidateMode?>('autovalidateMode', autovalidateMode),
    );
    properties.add(
      ObjectFlagProperty<String? Function(String? p1)?>.has(
        'validator',
        validator,
      ),
    );
  }
}
