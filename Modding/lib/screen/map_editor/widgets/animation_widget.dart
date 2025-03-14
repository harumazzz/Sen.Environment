import 'dart:math';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:matrix4_transform/matrix4_transform.dart';
import '../bloc/ticker/ticker_bloc.dart';
import '../include/painter.dart';
import '../include/visual_animation.dart';

class AnimationWidget extends StatelessWidget {
  const AnimationWidget({
    super.key,
    required this.visual,
    required this.labelPlay,
    this.forceLabelPlay,
    this.borderRect,
    this.borderColor = Colors.white,
    this.borderWidth = 2.0,
    this.playSingleFrame = false,
    this.filterQuality = FilterQuality.high,
  });

  final VisualAnimation visual;

  final String? forceLabelPlay;

  final Iterable<String> labelPlay;

  final Rect? borderRect;

  final bool playSingleFrame;

  final double borderWidth;

  final Color borderColor;

  final FilterQuality filterQuality;

  @override
  Widget build(BuildContext context) {
    final spriteFrame = visual.spriteFrame;
    if (labelPlay.isNotEmpty && !labelPlay.contains(visual.labelPlay)) {
      visual.labelPlay = labelPlay.first;
    }
    Widget child;
    if (spriteFrame.length == 1) {
      child = spriteFrame.first;
    } else if (playSingleFrame) {
      final labelInfo =
          visual.labelInfo[forceLabelPlay ?? visual.labelPlay] ??
          visual.labelInfo['main']!;
      child = spriteFrame.elementAt(labelInfo.begin);
    } else {
      child = BlocBuilder<TickerBloc, TickerState>(
        builder: (context, state) {
          final labelInfo =
              visual.labelInfo[forceLabelPlay ?? visual.labelPlay] ??
              visual.labelInfo['main']!;
          final duration = labelInfo.end - labelInfo.begin;
          final frameIndex = (state.tick % duration) + labelInfo.begin;
          if (forceLabelPlay == null &&
              labelPlay.length > 1 &&
              frameIndex == labelInfo.end - 1) {
            visual.labelPlay = labelPlay.elementAt(
              Random().nextInt(labelPlay.length),
            );
          }
          return spriteFrame.elementAt(frameIndex);
        },
      );
    }
    if (borderWidth > 0) {
      child = CustomPaint(
        painter: BorderPainer(
          itemRect:
              borderRect ??
              Rect.fromLTWH(
                0,
                0,
                visual.visualSize.width,
                visual.visualSize.height,
              ),
          borderColor: borderColor,
          borderWidth: borderWidth,
          filterQuality: filterQuality,
        ),
        child: child,
      );
    }
    return child;
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<VisualAnimation>('visual', visual));
    properties.add(StringProperty('forceLabelPlay', forceLabelPlay));
    properties.add(IterableProperty<String>('labelPlay', labelPlay));
    properties.add(DiagnosticsProperty<Rect?>('borderRect', borderRect));
    properties.add(
      DiagnosticsProperty<bool>('playSingleFrame', playSingleFrame),
    );
    properties.add(DoubleProperty('borderWidth', borderWidth));
    properties.add(ColorProperty('borderColor', borderColor));
    properties.add(EnumProperty<FilterQuality>('filterQuality', filterQuality));
  }
}

class AnimationRotateWidget extends StatelessWidget {
  const AnimationRotateWidget({
    super.key,
    required this.matrix,
    required this.rotationRate,
    required this.rotateOrigin,
    required this.child,
  });

  final Offset rotateOrigin;

  final double rotationRate;

  final Matrix4 matrix;

  final Widget child;

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<TickerBloc, TickerState>(
      builder: (context, state) {
        final rotateDeg = (state.tick * rotationRate * 1.7) % 360;
        return Transform(
          alignment: Alignment.topLeft,
          transform: matrix.multiplied(
            Matrix4Transform()
                .rotateDegrees(-rotateDeg, origin: rotateOrigin)
                .m,
          ),
          child: child,
        );
      },
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Offset>('rotateOrigin', rotateOrigin));
    properties.add(DoubleProperty('rotationRate', rotationRate));
    properties.add(TransformProperty('matrix', matrix));
  }
}
