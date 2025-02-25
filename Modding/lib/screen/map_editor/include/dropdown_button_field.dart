import 'package:flutter/material.dart';

class DropdownButtonField<T> extends StatelessWidget {
  const DropdownButtonField(
      {super.key,
      required this.label,
      required this.value,
      required this.items,
      required this.onChanged});

  final String label;

  final T value;

  final List<DropdownMenuItem<T>> items;

  final void Function(T?) onChanged;

  @override
  Widget build(BuildContext context) {
    return DropdownButtonFormField<T>(
        value: value,
        isExpanded: true,
        focusColor: Colors.transparent,
        decoration: InputDecoration(
          labelText: label,
          labelStyle: const TextStyle(fontWeight: FontWeight.bold),
          contentPadding: const EdgeInsets.fromLTRB(0, 4, 0, 12),
          floatingLabelBehavior: FloatingLabelBehavior.always,
        ),
        items: items,
        onChanged: onChanged);
  }
}
