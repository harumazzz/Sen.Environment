import 'dart:async';

import 'package:drop_down_search_field/drop_down_search_field.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';

class DowndropSearchField extends StatelessWidget {
  const DowndropSearchField({
    super.key,
    required this.label,
    required this.value,
    required this.suggestionsCallback,
    required this.itemBuilder,
    required this.onSuggestionSelected,
    this.autovalidateMode = AutovalidateMode.onUnfocus,
    this.validator,
  });

  final String label;

  final String value;

  final FutureOr<Iterable<String>> Function(String)? suggestionsCallback;

  final Widget Function(BuildContext, String) itemBuilder;

  final void Function(String) onSuggestionSelected;

  final AutovalidateMode autovalidateMode;

  final String? Function(String?)? validator;

  @override
  Widget build(BuildContext context) {
    final controller = TextEditingController(text: value);
    return DropDownSearchFormField(
      textFieldConfiguration: TextFieldConfiguration(
        minLines: 1,
        maxLines: null,
        keyboardType: TextInputType.multiline,
        decoration: InputDecoration(
          labelText: label,
          labelStyle: const TextStyle(fontWeight: FontWeight.bold),
          contentPadding: const EdgeInsets.fromLTRB(0, 4, 0, 12),
          floatingLabelBehavior: FloatingLabelBehavior.always,
        ),
        controller: controller,
      ),
      suggestionsCallback: suggestionsCallback,
      itemBuilder: itemBuilder,
      itemSeparatorBuilder: (context, index) {
        return const Divider();
      },
      transitionBuilder: (context, suggestionsBox, controller) {
        return Container(
          decoration: BoxDecoration(borderRadius: BorderRadius.circular(30)),
          child: suggestionsBox,
        );
      },
      onSuggestionSelected: (value) {
        controller.text = value;
        onSuggestionSelected(value);
      },
      autovalidateMode: autovalidateMode,
      validator: validator,
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('label', label));
    properties.add(StringProperty('value', value));
    properties.add(
      ObjectFlagProperty<FutureOr<Iterable<String>> Function(String p1)?>.has(
        'suggestionsCallback',
        suggestionsCallback,
      ),
    );
    properties.add(
      ObjectFlagProperty<Widget Function(BuildContext p1, String p2)>.has(
        'itemBuilder',
        itemBuilder,
      ),
    );
    properties.add(
      ObjectFlagProperty<void Function(String p1)>.has(
        'onSuggestionSelected',
        onSuggestionSelected,
      ),
    );
    properties.add(
      EnumProperty<AutovalidateMode>('autovalidateMode', autovalidateMode),
    );
    properties.add(
      ObjectFlagProperty<String? Function(String? p1)?>.has(
        'validator',
        validator,
      ),
    );
  }
}
