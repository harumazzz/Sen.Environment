import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/model/message.dart';

class MessageCard extends StatelessWidget {
  final Message message;

  const MessageCard({
    super.key,
    required this.message,
  });

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: Card(
        elevation: 2.0,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(16.0),
        ),
        color: _color(message, context),
        child: ListTile(
          leading: const Icon(
            Symbols.terminal,
            size: 20.0,
          ),
          title: Text(
            message.title,
            style: Theme.of(context).textTheme.labelMedium!.copyWith(
                  fontWeight: FontWeight.w600,
                ),
          ),
          subtitle: message.subtitle != null && message.subtitle!.isNotEmpty
              ? Text(
                  message.subtitle!,
                  style: Theme.of(context).textTheme.labelSmall,
                )
              : null,
        ),
      ),
    );
  }

  Color? _color(Message message, BuildContext context) {
    if (message.color == null) return null;
    final alpha = Theme.of(context).brightness == Brightness.dark ? 0.36 : 0.62;
    final color = message.exchangeColor(context);
    if (Theme.of(context).brightness == Brightness.dark || color != Theme.of(context).colorScheme.secondaryContainer) {
      return color.withValues(alpha: alpha);
    }
    return color;
  }
}
