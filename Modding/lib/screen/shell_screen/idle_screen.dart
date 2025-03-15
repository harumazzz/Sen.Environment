import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'dart:async';

import 'package:loading_animation_widget/loading_animation_widget.dart';

class IdleScreen extends StatefulWidget {
  const IdleScreen({
    super.key,
    this.size = 150,
    this.color = Colors.blue,
    this.scaleFactor = 1.5,
    required this.child,
    required this.text,
  });
  final double size;
  final Color color;
  final double scaleFactor;
  final Widget child;
  final String text;

  @override
  State<IdleScreen> createState() => _IdleScreenState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DoubleProperty('size', size));
    properties.add(ColorProperty('color', color));
    properties.add(DoubleProperty('scaleFactor', scaleFactor));
    properties.add(StringProperty('text', text));
  }
}

class _IdleScreenState extends State<IdleScreen> with TickerProviderStateMixin {
  late AnimationController _pulseController;
  late AnimationController _childController;
  late AnimationController _textController;
  late Animation<double> _pulseAnimation;
  late Animation<double> _textAnimation;
  final List<double> _circleScales = [0.5, 1.0, 1.5, 2.0];

  @override
  void initState() {
    super.initState();
    _pulseController = AnimationController(
      vsync: this,
      duration: const Duration(seconds: 2),
    )..repeat(reverse: true);
    _pulseAnimation = Tween<double>(
      begin: 0.0,
      end: 1.0,
    ).animate(_pulseController);
    _childController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 800),
      lowerBound: 0.9,
      upperBound: 1.1,
    )..repeat(reverse: true);
    _textController = AnimationController(
      vsync: this,
      duration: const Duration(seconds: 1),
    )..repeat(reverse: true);
    _textAnimation = Tween<double>(
      begin: 0.3,
      end: 1.0,
    ).animate(_textController);
    Timer.periodic(const Duration(milliseconds: 600), (timer) {
      if (mounted) {
        setState(() {
          _circleScales.insert(0, _circleScales.removeLast());
        });
      }
    });
  }

  @override
  void dispose() {
    _pulseController.dispose();
    _childController.dispose();
    _textController.dispose();
    super.dispose();
  }

  Widget _buildPulse(double scale, int delay) {
    return AnimatedBuilder(
      animation: _pulseAnimation,
      builder: (context, child) {
        return Transform.scale(
          scale: scale * widget.scaleFactor + _pulseAnimation.value * 0.8,
          child: Container(
            width: widget.size * 0.8,
            height: widget.size * 0.8,
            decoration: BoxDecoration(
              shape: BoxShape.circle,
              color: widget.color.withValues(alpha: 0.15),
            ),
          ),
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        spacing: 10.0,
        children: [
          Stack(
            alignment: Alignment.center,
            children: [
              for (var i = 0; i < _circleScales.length; i++)
                _buildPulse(_circleScales[i], i * 150),
              ScaleTransition(
                scale: _childController,
                child: Container(
                  width: widget.size,
                  height: widget.size,
                  decoration: BoxDecoration(
                    shape: BoxShape.circle,
                    border: Border.all(color: widget.color, width: 3),
                  ),
                  child: widget.child,
                ),
              ),
            ],
          ),
          FadeTransition(
            opacity: _textAnimation,
            child: Row(
              crossAxisAlignment: CrossAxisAlignment.end,
              spacing: 4.0,
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Text(
                  widget.text,
                  style: TextStyle(
                    fontSize: 16,
                    fontWeight: FontWeight.bold,
                    color: Theme.of(context).colorScheme.secondary,
                  ),
                ),
                LoadingAnimationWidget.waveDots(
                  color: Theme.of(context).colorScheme.secondary,
                  size: 16.0,
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
