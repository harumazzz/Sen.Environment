import 'package:flutter/material.dart';

class TextStringField extends StatelessWidget {
  const TextStringField(
      {super.key,
      required this.label,
      required this.value,
      required this.onFieldSubmitted,
      this.autovalidateMode,
      this.validator});

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
          contentPadding: const EdgeInsets.fromLTRB(0, 4, 0, 12),
          floatingLabelBehavior: FloatingLabelBehavior.always),
      /*
      onTapOutside: (_) {
        textController.text = value ?? '';
      },
      */
      autovalidateMode: autovalidateMode,
      validator: validator,
      onFieldSubmitted: onFieldSubmitted,
    );
  }
}
