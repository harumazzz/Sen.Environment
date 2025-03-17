import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:loading_animation_widget/loading_animation_widget.dart';

import '../../extension/context.dart';

class IdleScreen extends StatefulWidget {
  const IdleScreen({
    super.key,
    this.size = 100,
    this.color = Colors.blue,
    this.scaleFactor = 1.4,
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
  late AnimationController _shimmerController;
  late Animation<double> _pulseAnimation;
  late Animation<double> _shimmerAnimation;
  final List<double> _circleScales = [0.5, 1.0, 1.5];

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
    _shimmerController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 1500),
    )..repeat();
    _shimmerAnimation = Tween<double>(begin: -2, end: 2).animate(
      CurvedAnimation(parent: _shimmerController, curve: Curves.easeInOut),
    );
  }

  @override
  void dispose() {
    _pulseController.dispose();
    _shimmerController.dispose();
    super.dispose();
  }

  Widget _buildPulse(double scale, double maxSize) {
    return AnimatedBuilder(
      animation: _pulseAnimation,
      builder: (context, child) {
        return Transform.scale(
          scale: scale * widget.scaleFactor + _pulseAnimation.value * 0.5,
          child: Container(
            width: maxSize * 0.8,
            height: maxSize * 0.8,
            decoration: BoxDecoration(
              shape: BoxShape.circle,
              color: widget.color.withValues(alpha: 0.2),
            ),
          ),
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    final double screenWidth = MediaQuery.sizeOf(context).width;
    final maxSize = (screenWidth < 400) ? screenWidth * 0.4 : widget.size;
    final textStyle = Theme.of(context).textTheme.headlineSmall!.copyWith(
      color: Theme.of(context).colorScheme.onSurface,
      fontWeight: FontWeight.bold,
    );
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        spacing: 100.0,
        children: [
          AnimatedBuilder(
            animation: _shimmerAnimation,
            builder: (context, child) {
              return Row(
                crossAxisAlignment: CrossAxisAlignment.end,
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  ShaderMask(
                    shaderCallback: (bounds) {
                      return LinearGradient(
                        colors: [
                          context.isDarkMode
                              ? Colors.white.withValues(alpha: 0.3)
                              : Theme.of(
                                context,
                              ).colorScheme.secondary.withValues(alpha: 0.82),
                          Theme.of(context).colorScheme.primary,
                          context.isDarkMode
                              ? Colors.white.withValues(alpha: 0.3)
                              : Theme.of(
                                context,
                              ).colorScheme.secondary.withValues(alpha: 0.82),
                        ],
                        stops: const [0.0, 0.5, 1.0],
                        begin: Alignment(_shimmerAnimation.value, 0),
                        end: Alignment(_shimmerAnimation.value + 1.5, 0),
                      ).createShader(bounds);
                    },
                    blendMode: BlendMode.srcIn,
                    child: Row(
                      spacing: 4.0,
                      crossAxisAlignment: CrossAxisAlignment.end,
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        Text(
                          widget.text,
                          style: textStyle.copyWith(
                            color: Colors.white, // Ensures shimmer stands out
                          ),
                        ),
                        LoadingAnimationWidget.waveDots(
                          color: Theme.of(context).colorScheme.primary,
                          size: textStyle.fontSize! - 6,
                        ),
                      ],
                    ),
                  ),
                ],
              );
            },
          ),
          Stack(
            alignment: Alignment.center,
            children: [
              for (final scale in _circleScales) _buildPulse(scale, maxSize),
              SizedBox(width: maxSize, height: maxSize, child: widget.child),
            ],
          ),
        ],
      ),
    );
  }
}
