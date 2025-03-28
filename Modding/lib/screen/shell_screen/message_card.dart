import 'dart:io';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:path/path.dart' as p;

import '../../extension/color.dart';
import '../../extension/context.dart';
import '../../extension/platform.dart';
import '../../model/message.dart';
import '../../service/file_helper.dart';

class MessageCard extends StatelessWidget {
  const MessageCard({super.key, required this.message});
  final Message message;

  @override
  Widget build(BuildContext context) {
    final color = _getCardColor(context);
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: Card(
        elevation: context.isDarkMode ? 1 : 3,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(16.0),
        ),
        color: color,
        clipBehavior: Clip.antiAlias,
        child: Padding(
          padding: const EdgeInsets.symmetric(horizontal: 12.0, vertical: 12.0),
          child: Row(
            children: [
              Icon(
                message.icon,
                size: 32.0,
                color: Theme.of(context).colorScheme.onSurfaceVariant,
              ),
              const SizedBox(width: 8.0),
              Expanded(child: MessageContent(message: message)),
              const SizedBox(width: 8.0),
            ],
          ),
        ),
      ),
    );
  }

  Color _getCardColor(BuildContext context) {
    final theme = Theme.of(context);
    final baseColor = message.exchangeColor(context);

    return theme.brightness == Brightness.dark
        ? baseColor
            .withValues(alpha: 0.2)
            .blendWith(theme.colorScheme.surface, 0.7)
        : baseColor
            .withValues(alpha: 0.85)
            .blendWith(theme.colorScheme.surface, 0.5);
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Message>('message', message));
  }
}

class MessageContent extends StatelessWidget {
  const MessageContent({super.key, required this.message});
  final Message message;

  @override
  Widget build(BuildContext context) {
    final textColor = Theme.of(context).colorScheme.onSurface;
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      mainAxisSize: MainAxisSize.min,
      children: [
        Text(
          message.validTitle,
          style: Theme.of(context).textTheme.titleSmall!.copyWith(
            fontWeight: FontWeight.bold,
            color: textColor,
          ),
        ),
        if (message.hasMessage) SelectableMessageText(message: message),
      ],
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Message>('message', message));
  }
}

class SelectableMessageText extends StatelessWidget {
  const SelectableMessageText({required this.message, super.key});
  final Message message;

  @override
  Widget build(BuildContext context) {
    final isDarkMode = Theme.of(context).brightness == Brightness.dark;
    return Padding(
      padding: const EdgeInsets.only(top: 4.0),
      child: SelectableText(
        message.subtitle!,
        style: Theme.of(context).textTheme.bodySmall!.copyWith(
          color:
              isDarkMode
                  ? Colors.grey.shade400
                  : Theme.of(context).colorScheme.onSurfaceVariant,
        ),
        contextMenuBuilder: (context, editableTextState) {
          return AdaptiveTextSelectionToolbar.buttonItems(
            anchors: editableTextState.contextMenuAnchors,
            buttonItems: _getContextMenuItems(
              context,
              editableTextState,
              message.subtitle!,
            ),
          );
        },
      ),
    );
  }

  static List<ContextMenuButtonItem> _getContextMenuItems(
    BuildContext context,
    EditableTextState editableTextState,
    String subtitle,
  ) {
    final buttonItems = editableTextState.contextMenuButtonItems;
    if (CurrentPlatform.isDesktop) {
      if (FileSystemEntity.typeSync(subtitle) !=
          FileSystemEntityType.notFound) {
        buttonItems.add(
          ContextMenuButtonItem(
            label: context.los.open,
            onPressed: () async {
              if (FileHelper.isFile(subtitle)) {
                await FileHelper.revealFile(p.dirname(subtitle));
              } else {
                await FileHelper.revealFile(subtitle);
              }
            },
          ),
        );
      }
    }
    return buttonItems;
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Message>('message', message));
  }
}
