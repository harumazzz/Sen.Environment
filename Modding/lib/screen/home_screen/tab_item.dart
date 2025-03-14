import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';

class TabItem extends Equatable {
  final String title;
  final Widget widget;
  final Widget icon;

  const TabItem({
    required this.title,
    required this.widget,
    required this.icon,
  });

  @override
  List<Object?> get props => [title, widget, icon];
}
