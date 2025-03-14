import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../bloc/history/history_bloc.dart';
import '../bloc/toolbar/toolbar_bloc.dart';

class ScreenShortcut extends StatelessWidget {
  const ScreenShortcut({super.key});

  @override
  Widget build(BuildContext context) {
    final colorScheme = Theme.of(context).colorScheme;
    final buttonColor = colorScheme.surfaceContainerLow;
    final selectedColor = colorScheme.secondaryContainer;
    final disableColor =
        Theme.of(context).brightness == Brightness.light
            ? colorScheme.inverseSurface
            : colorScheme.outlineVariant;
    return Padding(
      padding: const EdgeInsets.all(8),
      child: Column(
        children: [
          BlocBuilder<HistoryBloc, HistoryState>(
            builder: (context, state) {
              final historyBloc = context.read<HistoryBloc>();
              final captureManager = state.captureManager;
              return ValueListenableBuilder(
                valueListenable: captureManager.notifier,
                builder: (context, notifierIndex, ref) {
                  return Row(
                    children: [
                      ShortcutItem(
                        iconData: Symbols.arrow_left_alt,
                        color:
                            state.captureManager.canUndo()
                                ? selectedColor
                                : disableColor,
                        onTap:
                            state.captureManager.canUndo()
                                ? () {
                                  historyBloc.add(const UndoEvent());
                                }
                                : null,
                      ),
                      ShortcutItem(
                        iconData: Symbols.arrow_right_alt,
                        color:
                            state.captureManager.canRedo()
                                ? selectedColor
                                : disableColor,
                        onTap:
                            state.captureManager.canRedo()
                                ? () {
                                  historyBloc.add(const RedoEvent());
                                }
                                : null,
                      ),
                    ],
                  );
                },
              );
            },
          ),
          BlocBuilder<ToolBarBloc, ToolBarState>(
            builder: (context, state) {
              return Row(
                children: [
                  ShortcutItem(
                    iconData: Symbols.pan_tool,
                    color:
                        state.toolStatus[ToolType.panTool]!
                            ? selectedColor
                            : buttonColor,
                    onTap: () {
                      context.read<ToolBarBloc>().add(
                        const ToolToogled(type: ToolType.panTool),
                      );
                    },
                  ),
                  ShortcutItem(
                    iconData: Symbols.gesture_select,
                    color:
                        state.toolStatus[ToolType.rectangleTool]!
                            ? selectedColor
                            : buttonColor,
                    onTap: () {
                      context.read<ToolBarBloc>().add(
                        const ToolToogled(type: ToolType.rectangleTool),
                      );
                    },
                  ),
                ],
              );
            },
          ),
        ],
      ),
    );
  }
}

class ShortcutItem extends StatelessWidget {
  const ShortcutItem({
    super.key,
    required this.iconData,
    required this.onTap,
    required this.color,
  });

  final IconData iconData;

  final void Function()? onTap;

  final Color color;

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(4),
      child: ClipOval(
        child: Material(
          color: color,
          child: InkWell(
            onTap: onTap,
            child: SizedBox(width: 40, height: 40, child: Icon(iconData)),
          ),
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<IconData>('iconData', iconData));
    properties.add(ObjectFlagProperty<void Function()?>.has('onTap', onTap));
    properties.add(ColorProperty('color', color));
  }
}
