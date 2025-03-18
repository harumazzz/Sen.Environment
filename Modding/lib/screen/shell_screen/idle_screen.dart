import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:loading_animation_widget/loading_animation_widget.dart';
import 'dart:math' as math;

import '../../extension/context.dart';

class IdleScreen extends StatefulWidget {
  const IdleScreen({super.key, this.size = 100});

  final double size;

  @override
  State<IdleScreen> createState() => _IdleScreenState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DoubleProperty('size', size));
  }
}

class _IdleScreenState extends State<IdleScreen> with TickerProviderStateMixin {
  late AnimationController _rotationController;
  late AnimationController _pulseController;
  late AnimationController _particleController;
  late Animation<double> _shimmerAnimation;
  late Animation<double> _pulseAnimation;
  late Animation<double> _particleAnimation;

  @override
  void initState() {
    super.initState();
    _rotationController = AnimationController(
      vsync: this,
      duration: const Duration(seconds: 2),
    )..repeat();
    _pulseController = AnimationController(
      vsync: this,
      duration: const Duration(seconds: 3),
    )..repeat(reverse: true);
    _particleController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 1500),
    )..repeat(reverse: true);
    _shimmerAnimation = Tween<double>(begin: -2, end: 2).animate(
      CurvedAnimation(parent: _rotationController, curve: Curves.easeInOut),
    );

    _pulseAnimation = Tween<double>(begin: 0.8, end: 1.2).animate(
      CurvedAnimation(parent: _pulseController, curve: Curves.easeInOut),
    );

    _particleAnimation = Tween<double>(begin: 0, end: 1).animate(
      CurvedAnimation(parent: _particleController, curve: Curves.easeInOut),
    );
  }

  @override
  void dispose() {
    _rotationController.dispose();
    _pulseController.dispose();
    _particleController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final textStyle = Theme.of(context).textTheme.headlineSmall!.copyWith(
      color: Theme.of(context).colorScheme.onSurface,
      fontWeight: FontWeight.bold,
    );

    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        spacing: 60.0,
        children: [
          AnimatedBuilder(
            animation: _shimmerAnimation,
            builder: (context, child) {
              return ShaderMask(
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
                  crossAxisAlignment: CrossAxisAlignment.end,
                  mainAxisAlignment: MainAxisAlignment.center,
                  spacing: 4.0,
                  children: [
                    Text(
                      context.los.sen_is_listening,
                      style: textStyle.copyWith(fontWeight: FontWeight.bold),
                    ),
                    LoadingAnimationWidget.waveDots(
                      color: Theme.of(context).colorScheme.primary,
                      size: textStyle.fontSize!,
                    ),
                  ],
                ),
              );
            },
          ),
          AnimatedBuilder(
            animation: Listenable.merge([
              _rotationController,
              _pulseAnimation,
              _particleAnimation,
            ]),
            builder: (context, child) {
              return Transform.rotate(
                angle: _rotationController.value * 2 * math.pi,
                child: CustomPaint(
                  size: Size(widget.size, widget.size),
                  painter: _CustomIconPainter(
                    Theme.of(context).colorScheme.primaryContainer,
                    _pulseAnimation.value,
                    _particleAnimation.value,
                  ),
                ),
              );
            },
          ),
        ],
      ),
    );
  }
}

class _CustomIconPainter extends CustomPainter {
  const _CustomIconPainter(this.color, this.pulseFactor, this.particleFactor);
  final Color color;
  final double pulseFactor;
  final double particleFactor;

  @override
  void paint(Canvas canvas, Size size) {
    final Paint glowPaint =
        Paint()
          ..color = color.withValues(alpha: 0.3)
          ..maskFilter = const MaskFilter.blur(BlurStyle.normal, 15);

    final Paint orbPaint =
        Paint()
          ..color = color.withValues(alpha: 0.7)
          ..style = PaintingStyle.fill;

    final Paint ringPaint =
        Paint()
          ..color = color.withValues(alpha: 0.5)
          ..style = PaintingStyle.stroke
          ..strokeWidth = 2;

    final double r = size.width / 2;
    final double glowRadius = r * 0.9 * pulseFactor;
    final double orbRadius = r * 0.8 * pulseFactor;

    canvas.drawCircle(Offset(r, r), glowRadius, glowPaint);
    canvas.drawCircle(Offset(r, r), orbRadius, orbPaint);

    for (double i = 1.1; i < 1.5; i += 0.15) {
      canvas.drawCircle(Offset(r, r), r * i * pulseFactor, ringPaint);
    }

    final Paint particlePaint =
        Paint()
          ..color = color.withValues(alpha: particleFactor)
          ..style = PaintingStyle.fill;

    for (int i = 0; i < 6; i++) {
      final double angle = (math.pi / 3) * i;
      final double px = r + math.cos(angle) * (r * 1.2);
      final double py = r + math.sin(angle) * (r * 1.2);
      canvas.drawCircle(Offset(px, py), 4, particlePaint);
    }
  }

  @override
  bool shouldRepaint(covariant CustomPainter oldDelegate) => true;
}
