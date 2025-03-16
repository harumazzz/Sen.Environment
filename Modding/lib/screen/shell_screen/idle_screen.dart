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
  late AnimationController _shimmerController;
  late Animation<double> _pulseAnimation;
  late Animation<double> _shimmerAnimation;
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
    _shimmerController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 1500),
    )..repeat();
    _shimmerAnimation = Tween<double>(begin: -2, end: 2).animate(
      CurvedAnimation(parent: _shimmerController, curve: Curves.easeInOut),
    );
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
    _shimmerController.dispose();
    super.dispose();
  }

  Widget _buildPulse(double scale, int delay, double maxSize) {
    return AnimatedBuilder(
      animation: _pulseAnimation,
      builder: (context, child) {
        return Transform.scale(
          scale: scale * widget.scaleFactor + _pulseAnimation.value * 0.8,
          child: Container(
            width: maxSize * 0.8,
            height: maxSize * 0.8,
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
    final double screenWidth = MediaQuery.of(context).size.width;
    double maxSize = (screenWidth < 400) ? screenWidth * 0.4 : widget.size;
    double adjustedTextSize = (screenWidth < 400) ? 14.0 : 16.0;
    return Center(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          Stack(
            alignment: Alignment.center,
            children: [
              for (var i = 0; i < _circleScales.length; i++)
                _buildPulse(_circleScales[i], i * 150, maxSize),
              ScaleTransition(
                scale: _childController,
                child: Container(
                  width: maxSize,
                  height: maxSize,
                  decoration: BoxDecoration(
                    shape: BoxShape.circle,
                    border: Border.all(color: widget.color, width: 3),
                  ),
                  child: widget.child,
                ),
              ),
            ],
          ),
          const SizedBox(height: 10),
          AnimatedBuilder(
            animation: _shimmerAnimation,
            builder: (context, child) {
              return ShaderMask(
                shaderCallback: (bounds) {
                  return LinearGradient(
                    colors: [
                      Colors.white.withValues(alpha: 0.2),
                      Colors.white.withValues(alpha: 0.8),
                      Colors.white.withValues(alpha: 0.2),
                    ],
                    stops: const [0.0, 0.5, 1.0],
                    begin: Alignment(_shimmerAnimation.value, 0),
                    end: Alignment(_shimmerAnimation.value + 1.5, 0),
                  ).createShader(bounds);
                },
                child: child,
              );
            },
            child: Row(
              crossAxisAlignment: CrossAxisAlignment.end,
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Text(
                  widget.text,
                  style: TextStyle(
                    fontSize: adjustedTextSize,
                    fontWeight: FontWeight.bold,
                    color: Theme.of(context).colorScheme.secondary,
                  ),
                ),
                LoadingAnimationWidget.waveDots(
                  color: Theme.of(context).colorScheme.secondary,
                  size: adjustedTextSize,
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
