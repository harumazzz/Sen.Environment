import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:sen/screen/map_editor/include/controller.dart';

class HotkeyListener extends StatelessWidget {
  const HotkeyListener({
    super.key,
    required this.controller,
    required this.child,
    this.onKeyDownEvent,
    this.onKeyUpEvent,
  });

  final Widget child;

  final CanvasController controller;

  final void Function(LogicalKeyboardKey)? onKeyDownEvent;

  final void Function(LogicalKeyboardKey)? onKeyUpEvent;

  @override
  Widget build(BuildContext context) {
    return KeyboardListener(
        // autofocus: true,
        focusNode: controller.focusNode,
        onKeyEvent: (event) {
          if (event is KeyDownEvent) {
            if (event.logicalKey == LogicalKeyboardKey.shiftLeft ||
                event.logicalKey == LogicalKeyboardKey.shiftRight) {
              controller.shiftPressed = true;
            }
            if (event.logicalKey == LogicalKeyboardKey.controlLeft ||
                event.logicalKey == LogicalKeyboardKey.controlRight) {
              controller.controlPressed = true;
            }
            if (event.logicalKey == LogicalKeyboardKey.metaLeft ||
                event.logicalKey == LogicalKeyboardKey.metaRight) {
              controller.metaPressed = true;
            }
            if (event.logicalKey == LogicalKeyboardKey.space) {
              controller.spacePressed = true;
            }
            if (onKeyDownEvent != null) {
              onKeyDownEvent!(event.logicalKey);
            }
          }
          if (event is KeyUpEvent) {
            if (onKeyUpEvent != null) {
              onKeyUpEvent!(event.logicalKey);
            }
            if (event.logicalKey == LogicalKeyboardKey.shiftLeft ||
                event.logicalKey == LogicalKeyboardKey.shiftRight) {
              controller.shiftPressed = false;
            }
            if (event.logicalKey == LogicalKeyboardKey.metaLeft ||
                event.logicalKey == LogicalKeyboardKey.metaRight) {
              controller.metaPressed = false;
            }
            if (event.logicalKey == LogicalKeyboardKey.controlLeft ||
                event.logicalKey == LogicalKeyboardKey.controlRight) {
              controller.controlPressed = false;
            }
            if (event.logicalKey == LogicalKeyboardKey.space) {
              controller.spacePressed = false;
            }
          }
        },
        child: child);
  }
}
