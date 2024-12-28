import 'package:flutter/material.dart';

class Item {
  final String title;
  final String description;
  final Widget icon;
  final bool isEnabled;
  bool selected;
  late void Function()? onSetting;
  late Widget Function() onWidget;

  Item({
    required this.title,
    required this.description,
    required this.icon,
    required this.isEnabled,
    this.onSetting,
    this.selected = false,
  });

  bool get isDisabled => !isEnabled;
}
