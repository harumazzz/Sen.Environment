import 'dart:math';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:matrix4_transform/matrix4_transform.dart';
import '../bloc/ticker/ticker_bloc.dart';
import 'painter.dart';

class HoverBox extends StatelessWidget {
  const HoverBox({super.key, required this.matrix, required this.rect});

  final Matrix4 matrix;

  final Rect rect;

  Color _getRandomWeightedColor(Map<Color, double> weights) {
    final random = Random();
    final totalWeight = weights.values.reduce((a, b) => a + b);
    var cumulativeWeight = 0.0;
    var randomValue = random.nextDouble() * totalWeight;
    for (final entry in weights.entries) {
      cumulativeWeight += entry.value;
      if (randomValue < cumulativeWeight) {
        return entry.key;
      }
    }
    throw Exception('No number selected. Check your weights.');
  }

  @override
  Widget build(BuildContext context) {
    final sizeHeight = rect.height;
    final sizeWidth = rect.width;
    final scaleXRatio = (sizeWidth + 100) / sizeWidth;
    final scaleYRatio = (sizeHeight + 100) / sizeHeight;
    final begin = Matrix4Transform.from(matrix);
    final end = Matrix4Transform.from(matrix).scaleBy(
      x: scaleXRatio > 1.3 ? 1.3 : scaleXRatio,
      y: scaleYRatio > 1.3 ? 1.3 : scaleYRatio,
      origin: rect.center,
    );
    final matrixTween = Matrix4TransformTween(begin: begin, end: end);
    final opacityTween = IntTween(begin: 0, end: 10);
    final colors = <Color, double>{
      Colors.yellow: 0.8,
      Colors.red: 0.2,
      Colors.orange: 0.1,
      Colors.lightBlue: 0.1,
      Colors.white: 0.1,
      Colors.green: 0.1,
    };
    final color = _getRandomWeightedColor(colors);
    const opacityList = <double>[
      1,
      1,
      1,
      1,
      0.8,
      0.8,
      0.6,
      0.6,
      0.4,
      0.2,
      0.2,
      0.1,
      0.1,
    ];
    return BlocBuilder<TickerBloc, TickerState>(
      builder: (context, state) {
        final t = (state.tick % 14) * 0.09;
        return Opacity(
          opacity: opacityList[opacityTween.transform(t)],
          child: CustomPaint(
            painter: SelectPainer(
              matrix: matrixTween.transform(t).m,
              itemRect: rect,
              color: color,
            ),
          ),
        );
      },
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(TransformProperty('matrix', matrix));
    properties.add(DiagnosticsProperty<Rect>('rect', rect));
  }
}
