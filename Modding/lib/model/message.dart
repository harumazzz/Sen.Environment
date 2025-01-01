import 'package:flutter/material.dart';

class Message {
  final String title;
  final String? subtitle;
  final String? color;

  Message({
    required this.title,
    this.subtitle,
    this.color,
  });

  Color exchangeColor(BuildContext context) {
    return switch (color) {
      'red' => Colors.red,
      'green' => Colors.green,
      'yellow' => Colors.yellow,
      'cyan' => Colors.cyan,
      'default' => _getColorBasedOnTheme(context),
      _ => _getColorBasedOnTheme(context),
    };
  }

  Color _getColorBasedOnTheme(BuildContext context) {
    return Theme.of(context).colorScheme.secondaryContainer;
  }
}
