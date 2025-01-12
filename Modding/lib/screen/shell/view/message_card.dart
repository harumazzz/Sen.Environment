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
    final hasMessage = message.subtitle != null && message.subtitle!.isNotEmpty;
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 6.0),
      child: Card(
        elevation: 2.0,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(12.0),
        ),
        color: _color(message, context),
        child: Padding(
          padding: const EdgeInsets.symmetric(
            horizontal: 8.0,
            vertical: 12.0,
          ),
          child: Column(
            mainAxisSize: MainAxisSize.min,
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Row(
                children: [
                  const SizedBox(width: 4.0),
                  const Icon(
                    Symbols.terminal,
                    size: 24.0,
                  ),
                  const SizedBox(width: 12.0),
                  Expanded(
                    child: Column(
                      mainAxisSize: MainAxisSize.min,
                      crossAxisAlignment: CrossAxisAlignment.start,
                      mainAxisAlignment: MainAxisAlignment.start,
                      children: [
                        Text(
                          message.title,
                          style: Theme.of(context).textTheme.labelMedium!.copyWith(
                                fontWeight: FontWeight.w600,
                              ),
                        ),
                        if (hasMessage && message.subtitle != null) ...[
                          const SizedBox(height: 4.0),
                          Text(
                            message.subtitle!,
                            style: Theme.of(context).textTheme.labelSmall,
                          ),
                        ],
                      ],
                    ),
                  ),
                ],
              ),
            ],
          ),
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
