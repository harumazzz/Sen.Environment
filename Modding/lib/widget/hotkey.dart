import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import '../extension/platform.dart';
import 'package:window_manager/window_manager.dart';

class HotkeyBuilder extends StatefulWidget {
  const HotkeyBuilder({super.key, required this.child});

  final Widget child;

  @override
  State<HotkeyBuilder> createState() => _HotkeyBuilderState();
}

class _HotkeyBuilderState extends State<HotkeyBuilder> {
  Future<void> _setFullScreen() async {
    if (await windowManager.isFullScreen()) {
      await windowManager.setFullScreen(false);
    } else {
      await windowManager.setFullScreen(true);
    }
  }

  late final FocusNode _focusNode;

  @override
  void initState() {
    _focusNode = FocusNode();
    _focusNode.requestFocus();
    super.initState();
  }

  @override
  void dispose() {
    _focusNode.dispose();
    super.dispose();
  }

  void _keyEventPresed(KeyEvent keyboardEvent) async {
    if (keyboardEvent is KeyDownEvent) {
      final logicalKey = keyboardEvent.logicalKey;
      switch (logicalKey) {
        case LogicalKeyboardKey.f11:
          await _setFullScreen();
          break;
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    if (CurrentPlatform.isDesktop) {
      return KeyboardListener(
        focusNode: _focusNode,
        includeSemantics: false,
        onKeyEvent: _keyEventPresed,
        child: widget.child,
      );
    }
    return widget.child;
  }
}
