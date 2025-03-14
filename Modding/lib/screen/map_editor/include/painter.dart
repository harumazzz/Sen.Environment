import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'visual_image.dart';

class VisualImagePainer extends CustomPainter {
  const VisualImagePainer(
    this.property, {
    this.borderColor = Colors.white,
    this.borderWidth = 2.0,
    this.filterQuality = FilterQuality.high,
  });

  final (VisualImage, Matrix4?) property;

  final FilterQuality filterQuality;

  final double borderWidth;

  final Color borderColor;

  @override
  void paint(Canvas canvas, Size size) {
    final paint = Paint()..filterQuality = filterQuality;
    final image = property.$1;
    final matrix = property.$2;
    if (matrix != null) {
      canvas.transform(matrix.storage);
    }
    if (borderWidth > 0) {
      final borderPaint =
          Paint()
            ..filterQuality = filterQuality
            ..color = borderColor
            ..style = PaintingStyle.stroke
            ..strokeWidth = borderWidth;
      canvas.drawRect(
        Rect.fromLTWH(0, 0, image.width.toDouble(), image.height.toDouble()),
        borderPaint,
      );
    }
    canvas.drawImage(image, Offset.zero, paint);
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) {
    return false;
  }
}

class BorderPainer extends CustomPainter {
  const BorderPainer({
    required this.itemRect,
    required this.borderColor,
    this.borderWidth = 2.0,
    this.matrix,
    this.filterQuality = FilterQuality.high,
  });

  final Matrix4? matrix;

  final Rect itemRect;

  final Color borderColor;

  final double borderWidth;

  final FilterQuality filterQuality;

  @override
  void paint(Canvas canvas, Size size) {
    var scaleRatio = 1.0;
    if (matrix != null) {
      canvas.transform(matrix!.storage);
      scaleRatio = matrix!.getMaxScaleOnAxis();
    }
    if (borderWidth > 0.0) {
      final borderPaint =
          Paint()
            ..filterQuality = filterQuality
            ..color = borderColor
            ..style = PaintingStyle.stroke
            ..strokeWidth = borderWidth - borderWidth * scaleRatio;
      canvas.drawRect(itemRect, borderPaint);
    }
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) {
    return false;
  }
}

class SelectPainer extends CustomPainter {
  const SelectPainer({
    required this.matrix,
    required this.itemRect,
    this.color = Colors.yellow,
    this.filterQuality = FilterQuality.high,
  });

  final Matrix4 matrix;

  final Rect itemRect;

  final Color color;

  final FilterQuality filterQuality;

  @override
  void paint(Canvas canvas, Size size) {
    final borderPaint =
        Paint()
          ..filterQuality = filterQuality
          ..color = color
          ..style = PaintingStyle.stroke
          ..strokeWidth = 6.0;
    canvas.transform(matrix.storage);
    canvas.drawRect(itemRect, borderPaint);
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) {
    return true;
  }
}

class Marquee extends StatelessWidget {
  const Marquee({super.key, required this.start, required this.end});

  final Offset start, end;

  @override
  Widget build(BuildContext context) {
    final colors = Theme.of(context).colorScheme;
    return CustomPaint(
      painter: InlinePainter(
        brush:
            Paint()
              ..color = colors.secondary.withValues(alpha: 0.3)
              ..style = PaintingStyle.fill,
        builder: (brush, canvas, rect) {
          final marqueeRect = Rect.fromPoints(start, end);
          canvas.drawRect(marqueeRect, brush);
        },
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Offset>('start', start));
    properties.add(DiagnosticsProperty<Offset>('end', end));
  }
}

class InlinePainter extends CustomPainter {
  const InlinePainter({
    required this.brush,
    required this.builder,
    this.isAntiAlias = true,
  });
  final Paint brush;
  final bool isAntiAlias;
  final void Function(Paint paint, Canvas canvas, Rect rect) builder;

  @override
  void paint(Canvas canvas, Size size) {
    final rect = Offset.zero & size;
    brush.isAntiAlias = isAntiAlias;
    canvas.save();
    builder(brush, canvas, rect);
    canvas.restore();
  }

  @override
  bool shouldRepaint(covariant CustomPainter oldDelegate) {
    return true;
  }
}

/*
class RectangleInnerPainter extends CustomPainter {
  RectangleInnerPainter({required this.rect, this.filterQuality = FilterQuality.high});

  final Rect rect;

  final FilterQuality filterQuality;

  @override
  void paint(Canvas canvas, Size size) {
    final borderPaint = Paint()
      ..filterQuality = filterQuality
      ..color = Colors.red
      ..style = PaintingStyle.stroke
      ..strokeWidth = 3.0;

    canvas.drawRect(rect, borderPaint);
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) {
    return false;
  }
}
*/

class RectanglePainter extends CustomPainter {
  RectanglePainter({
    required this.innerRect,
    required this.outlineRect,
    this.boundingColor,
    this.filterQuality = FilterQuality.high,
  });

  final Rect innerRect;

  final Rect outlineRect;

  final Color? boundingColor;

  final FilterQuality filterQuality;

  @override
  void paint(Canvas canvas, Size size) {
    canvas.drawPath(
      Path.combine(
        PathOperation.difference,
        Path()..addRect(outlineRect),
        Path()
          ..addRect(innerRect)
          ..close(),
      ),
      Paint()..color = boundingColor ?? Colors.grey.withValues(alpha: 0.7),
    );
    final borderPaint =
        Paint()
          ..filterQuality = filterQuality
          ..color = Colors.red
          ..style = PaintingStyle.stroke
          ..strokeWidth = 3.0;

    canvas.drawRect(innerRect, borderPaint);
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) {
    return false;
  }
}
