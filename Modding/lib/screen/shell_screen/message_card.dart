import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../model/message.dart';

class MessageCard extends StatelessWidget {
  const MessageCard({super.key, required this.message});
  final Message message;

  @override
  Widget build(BuildContext context) {
    final hasMessage = message.subtitle != null && message.subtitle!.isNotEmpty;
    final theme = Theme.of(context);
    final color = _color(message, context);
    final textColor = theme.colorScheme.onSurface;
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: Card(
        elevation: 3,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(16.0),
        ),
        color: color,
        clipBehavior: Clip.antiAlias,
        child: Padding(
          padding: const EdgeInsets.symmetric(horizontal: 12.0, vertical: 12.0),
          child: Row(
            children: [
              const Icon(Symbols.terminal, size: 28.0),
              const SizedBox(width: 12.0),
              Expanded(
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  mainAxisSize: MainAxisSize.min,
                  children: [
                    SelectableText(
                      message.title,
                      style: theme.textTheme.titleSmall!.copyWith(
                        fontWeight: FontWeight.bold,
                        color: textColor,
                      ),
                    ),
                    if (hasMessage)
                      Padding(
                        padding: const EdgeInsets.only(top: 4.0),
                        child: SelectableText(
                          message.subtitle!,
                          style: theme.textTheme.bodySmall!.copyWith(
                            color: theme.colorScheme.onSurfaceVariant,
                          ),
                        ),
                      ),
                  ],
                ),
              ),
              const SizedBox(width: 8.0),
            ],
          ),
        ),
      ),
    );
  }

  Color? _color(Message message, BuildContext context) {
    final theme = Theme.of(context);
    final baseColor = message.exchangeColor(context);
    final isDark = theme.brightness == Brightness.dark;
    return isDark
        ? baseColor.withValues(alpha: 0.36)
        : baseColor.withValues(alpha: 0.82);
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Message>('message', message));
  }
}
