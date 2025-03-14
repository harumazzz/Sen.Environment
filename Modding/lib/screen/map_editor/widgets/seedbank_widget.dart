import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:matrix4_transform/matrix4_transform.dart';
import '../include/painter.dart';
import '../include/visual_image.dart';

class SeedBankWidget extends StatelessWidget {
  const SeedBankWidget({
    super.key,
    required this.plant,
    required this.seedtype,
    required this.matrix,
  });

  final VisualImage plant;

  final VisualImage seedtype;

  final Matrix4 matrix;

  @override
  Widget build(BuildContext context) {
    final posY = -(plant.height - 137.0);
    final plantMatrix = matrix.multiplied(
      Matrix4Transform().translate(x: 10, y: posY).m,
    );
    return CustomPaint(
      painter: VisualImagePainer((seedtype, matrix), borderWidth: 0),
      foregroundPainter: VisualImagePainer((
          plant,
          plantMatrix,
        ), borderWidth: 0,
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<VisualImage>('plant', plant));
    properties.add(DiagnosticsProperty<VisualImage>('seedtype', seedtype));
    properties.add(TransformProperty('matrix', matrix));
  }
}
