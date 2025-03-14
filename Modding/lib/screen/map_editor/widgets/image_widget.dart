import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:matrix4_transform/matrix4_transform.dart';
import '../bloc/ticker/ticker_bloc.dart';
import '../include/painter.dart';
import '../include/visual_image.dart';

class ImageWidget extends StatelessWidget {
  const ImageWidget({
    super.key,
    required this.image,
    required this.matrix,
    this.borderColor = Colors.white,
    this.borderWidth = 2.0,
    this.filterQuality = FilterQuality.high,
  });

  final VisualImage image;

  final double borderWidth;

  final Color borderColor;

  final Matrix4 matrix;

  final FilterQuality filterQuality;

  @override
  Widget build(BuildContext context) {
    return CustomPaint(
      painter: VisualImagePainer(
        (image, matrix),
        borderColor: borderColor,
        borderWidth: borderWidth,
        filterQuality: filterQuality,
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<VisualImage>('image', image));
    properties.add(DoubleProperty('borderWidth', borderWidth));
    properties.add(ColorProperty('borderColor', borderColor));
    properties.add(TransformProperty('matrix', matrix));
    properties.add(EnumProperty<FilterQuality>('filterQuality', filterQuality));
  }
}

class ImageRotateWidget extends StatelessWidget {
  const ImageRotateWidget({
    super.key,
    required this.image,
    required this.matrix,
    required this.rotationRate,
    required this.borderColor,
    this.borderWidth = 2.0,
    this.filterQuality = FilterQuality.high,
  });

  final VisualImage image;

  final double rotationRate;

  final Matrix4 matrix;

  final double borderWidth;

  final Color borderColor;

  final FilterQuality filterQuality;

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<TickerBloc, TickerState>(
      builder: (context, state) {
        final rotateDeg = (state.tick * rotationRate * 0.03) % 360;
        return CustomPaint(
          painter: VisualImagePainer(
            (
              image,
              matrix.multiplied(
                Matrix4Transform()
                    .rotateDegrees(
                      rotationRate > 0 ? -rotateDeg : rotateDeg,
                      origin: Offset(image.width / 2, image.height / 2),
                    )
                    .m,
              ),
            ),
            borderColor: borderColor,
            borderWidth: borderWidth,
            filterQuality: filterQuality,
          ),
        );
      },
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<VisualImage>('image', image));
    properties.add(DoubleProperty('rotationRate', rotationRate));
    properties.add(TransformProperty('matrix', matrix));
    properties.add(DoubleProperty('borderWidth', borderWidth));
    properties.add(ColorProperty('borderColor', borderColor));
    properties.add(EnumProperty<FilterQuality>('filterQuality', filterQuality));
  }
}
