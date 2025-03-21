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
    final theme = Theme.of(context);
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
              _buildIcon(theme),
              const SizedBox(width: 8.0),
              Expanded(child: _buildMessageContent(context, theme)),
              const SizedBox(width: 8.0),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildIcon(ThemeData theme) {
    return Icon(
      message.icon,
      size: 32.0,
      color: theme.colorScheme.onSurfaceVariant,
    );
  }

  Widget _buildMessageContent(BuildContext context, ThemeData theme) {
    final textColor = theme.colorScheme.onSurface;
    final isDarkMode = theme.brightness == Brightness.dark;
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      mainAxisSize: MainAxisSize.min,
      children: [
        Text(
          message.validTitle,
          style: theme.textTheme.titleSmall!.copyWith(
            fontWeight: FontWeight.bold,
            color: textColor,
          ),
        ),
        if (message.hasMessage)
          _buildSelectableText(context, theme, isDarkMode),
      ],
    );
  }

  Widget _buildSelectableText(
    BuildContext context,
    ThemeData theme,
    bool isDarkMode,
  ) {
    return Padding(
      padding: const EdgeInsets.only(top: 4.0),
      child: SelectableText(
        message.subtitle!,
        style: theme.textTheme.bodySmall!.copyWith(
          color:
              isDarkMode
                  ? Colors.grey.shade400
                  : theme.colorScheme.onSurfaceVariant,
        ),
        contextMenuBuilder: (context, editableTextState) {
          return AdaptiveTextSelectionToolbar.buttonItems(
            anchors: editableTextState.contextMenuAnchors,
            buttonItems: _getContextMenuItems(context, editableTextState),
          );
        },
      ),
    );
  }

  List<ContextMenuButtonItem> _getContextMenuItems(
    BuildContext context,
    EditableTextState editableTextState,
  ) {
    final buttonItems = editableTextState.contextMenuButtonItems;
    if (CurrentPlatform.isDesktop) {
      final current = message.subtitle!;
      if (FileSystemEntity.typeSync(current) != FileSystemEntityType.notFound) {
        buttonItems.add(
          ContextMenuButtonItem(
            label: context.los.open,
            onPressed: () async {
              if (FileHelper.isFile(current)) {
                await FileHelper.revealFile(p.dirname(current));
              } else {
                await FileHelper.revealFile(current);
              }
            },
          ),
        );
      }
    }
    return buttonItems;
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
