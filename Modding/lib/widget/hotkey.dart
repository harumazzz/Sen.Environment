import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:window_manager/window_manager.dart';

class HotkeyBuilder extends StatelessWidget {
  const HotkeyBuilder({
    super.key,
    required this.child,
  });

  final Widget child;

  Future<void> _setFullScreen() async {
    if (await windowManager.isFullScreen()) {
      await windowManager.setFullScreen(false);
    } else {
      await windowManager.setFullScreen(true);
    }
  }

  void _keyEventPresed(
    KeyEvent keyboardEvent,
  ) async {
    if (keyboardEvent is KeyDownEvent) {
      final logicalKey = keyboardEvent.logicalKey;
      switch (logicalKey) {
        case LogicalKeyboardKey.f11:
          _setFullScreen();
          break;
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      return KeyboardListener(
        focusNode: FocusNode(),
        autofocus: false,
        includeSemantics: false,
        onKeyEvent: _keyEventPresed,
        child: child,
      );
    }
    return child;
  }
}
