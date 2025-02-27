import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';

class Item extends Equatable {
  final String title;
  final String description;
  final Widget icon;
  final bool selected;
  final Function()? onSetting;
  final Function()? onWidget;

  const Item({
    required this.title,
    required this.description,
    required this.icon,
    this.onSetting,
    this.onWidget,
    this.selected = false,
  });

  @override
  List<Object?> get props {
    return [
      title,
      description,
      icon,
      selected,
      onSetting,
      onWidget,
    ];
  }
}
