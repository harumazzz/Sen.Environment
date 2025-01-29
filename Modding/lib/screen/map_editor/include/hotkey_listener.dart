import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:sen/screen/map_editor/include/controller.dart';
import 'package:window_manager/window_manager.dart';

class HotkeyListener extends StatelessWidget {
  const HotkeyListener({
    super.key,
    required this.controller,
    required this.child,
    this.onKeyDownEvent,
    this.onKeyRepeatEvent,
    this.onKeyUpEvent,
  });

  final Widget child;

  final CanvasController controller;

  final void Function(LogicalKeyboardKey)? onKeyDownEvent;

  final void Function(LogicalKeyboardKey)? onKeyRepeatEvent;

  final void Function(LogicalKeyboardKey)? onKeyUpEvent;

  Future<void> _setFullScreen() async {
    if (await windowManager.isFullScreen()) {
      await windowManager.setFullScreen(false);
    } else {
      await windowManager.setFullScreen(true);
    }
  }

  @override
  Widget build(BuildContext context) {
    return KeyboardListener(
        focusNode: controller.focusNode,
        onKeyEvent: (event) async {
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
            if (event.logicalKey == LogicalKeyboardKey.f11) {
              await _setFullScreen();
            }
            if (onKeyDownEvent != null) {
              onKeyDownEvent!(event.logicalKey);
            }
          } else if (event is KeyRepeatEvent) {
            if (onKeyRepeatEvent != null) {
              onKeyRepeatEvent!(event.logicalKey);
            }
          } else if (event is KeyUpEvent) {
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
