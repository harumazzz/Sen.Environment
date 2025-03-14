import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';

class Message extends Equatable {
  const Message({required this.title, this.color, this.subtitle});

  factory Message.fromJson(Map<String, dynamic> json) {
    return Message(
      title: json['title'] as String,
      color: json['color'] as String?,
      subtitle: json['subtitle'] as String?,
    );
  }
  final String title;
  final String? subtitle;
  final String? color;

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

  @override
  List<Object?> get props => [title, subtitle, color];

  Map<String, dynamic> toJson() {
    return {'title': title, 'subtitle': subtitle, 'color': color};
  }
}
