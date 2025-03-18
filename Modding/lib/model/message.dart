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

  String get validTitle => title.replaceAll(RegExp(r':(?=\s*$)'), '');

  Color exchangeColor(BuildContext context) {
    final theme = Theme.of(context);
    final scheme = theme.colorScheme;
    final isDark = theme.brightness == Brightness.dark;
    return switch (color) {
      'red' => isDark ? scheme.error : scheme.errorContainer,
      'green' => isDark ? Colors.teal.shade500 : Colors.lightGreen.shade400,
      'yellow' => isDark ? Colors.amber.shade400 : Colors.amber.shade800,
      'cyan' => isDark ? Colors.lightBlue.shade300 : Colors.cyan.shade400,
      'blue' => isDark ? scheme.primaryContainer : scheme.primary,
      _ => _getColorBasedOnTheme(theme, isDark),
    };
  }

  Color _getColorBasedOnTheme(ThemeData theme, bool isDark) {
    return isDark
        ? theme.colorScheme.onSurfaceVariant
        : theme.colorScheme.secondaryContainer;
  }

  bool get hasMessage => subtitle != null && subtitle!.isNotEmpty;

  @override
  List<Object?> get props => [title, subtitle, color];

  Map<String, dynamic> toJson() {
    return {'title': title, 'subtitle': subtitle, 'color': color};
  }
}
