import 'package:flutter/material.dart';

class Item {
  final String title;
  final String description;
  final Widget icon;
  final bool isEnabled;
  final bool selected;
  final Function()? onSetting;
  final Function()? onWidget;

  Item({
    required this.title,
    required this.description,
    required this.icon,
    required this.isEnabled,
    this.onSetting,
    this.onWidget,
    this.selected = false,
  });

  bool get isDisabled => !isEnabled;
}
