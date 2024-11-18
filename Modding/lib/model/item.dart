import 'package:flutter/material.dart';

class Item {
  final String title;
  final String description;
  final Widget icon;
  late void Function()? onSetting;
  late Widget Function() onWidget;

  Item({
    required this.title,
    required this.description,
    required this.icon,
    this.onSetting,
  });
}
