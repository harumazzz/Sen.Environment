import 'package:flutter/material.dart';
import 'dart:async';

class AnimatedButton extends StatefulWidget {
  final String message;
  final void Function() onClick;
  final Widget child;

  const AnimatedButton({
    super.key,
    required this.message,
    required this.onClick,
    required this.child,
  });

  @override
  State<AnimatedButton> createState() => _AnimatedButtonState();
}

class _AnimatedButtonState extends State<AnimatedButton> {
  bool _isPressed = false;
  Timer? _longPressTimer;

  @override
  void dispose() {
    _longPressTimer?.cancel();
    super.dispose();
  }

  void _startLongPress() {
    _longPressTimer = Timer(const Duration(milliseconds: 500), () {
      widget.onClick();
    });
  }

  void _cancelLongPress() {
    _longPressTimer?.cancel();
    setState(() {
      _isPressed = false;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Tooltip(
      message: widget.message,
      child: GestureDetector(
        onTap: widget.onClick,
        onLongPress: _startLongPress,
        onLongPressEnd: (_) => _cancelLongPress(),
        onLongPressUp: _cancelLongPress,
        child: AnimatedScale(
          scale: _isPressed ? 1.2 : 1.0,
          duration: const Duration(milliseconds: 400),
          child: IconButton(
            onPressed: widget.onClick,
            icon: widget.child,
          ),
        ),
      ),
    );
  }
}
