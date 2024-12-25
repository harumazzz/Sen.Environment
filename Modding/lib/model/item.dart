import 'package:flutter/material.dart';

class Item {
  final String title;
  final String description;
  final Widget icon;
  bool selected;
  late void Function()? onSetting;
  late Widget Function() onWidget;

  Item({
    required this.title,
    required this.description,
    required this.icon,
    this.onSetting,
    this.selected = false,
  });
}
