import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'scalable_item/scalable_item.dart';
import 'scalable_item/scale_direction_enum.dart';
import 'scalable_item/scale_helper.dart';
import 'scalable_item/scale_info.dart';

class RectangleBox extends StatefulWidget {
  const RectangleBox({
    super.key,
    required this.boundingRect,
    required this.onScalingEnd,
    this.minWidth = 0,
    this.minHeight = 0,
  });

  final Rect boundingRect;

  final double minWidth;

  final double minHeight;

  final Function(Rect updateRect) onScalingEnd;

  @override
  State<StatefulWidget> createState() => RectangleBoxState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Rect>('boundingRect', boundingRect));
    properties.add(DoubleProperty('minWidth', minWidth));
    properties.add(DoubleProperty('minHeight', minHeight));
    properties.add(
      ObjectFlagProperty<Function(Rect updateRect)>.has(
        'onScalingEnd',
        onScalingEnd,
      ),
    );
  }
}

class RectangleBoxState extends State<RectangleBox> {
  Rect _rect = Rect.zero;

  late final double _initX;

  late final double _initY;

  @override
  void initState() {
    _rect = widget.boundingRect;
    _initX = _rect.left;
    _initY = _rect.top;
    super.initState();
  }

  void _onScaling(DragUpdateDetails update, ScaleDirection scaleDirection) {
    final current = ScaleInfo(
      width: _rect.width,
      height: _rect.height,
      x: _rect.left,
      y: _rect.top,
    );
    final dx = update.delta.dx;
    final dy = update.delta.dy;
    final scaleInfoOpt = ScaleInfoOpt(
      shape: Shape.rectangle,
      scaleDirection: scaleDirection,
      dx: dx,
      dy: dy,
      rotateAngle: 0,
    );

    final scaleInfoAfterCalculation = ScaleHelper.getScaleInfo(
      current: current,
      options: scaleInfoOpt,
    );

    double updatedWidth = scaleInfoAfterCalculation.width;
    double updatedHeight = scaleInfoAfterCalculation.height;
    double updatedXPosition = scaleInfoAfterCalculation.x;
    double updatedYPosition = scaleInfoAfterCalculation.y;
    if (updatedWidth < widget.minWidth) {
      //updatedXPosition = _x;
      updatedWidth = widget.minWidth;
    }

    if (updatedHeight < widget.minHeight) {
      // updatedYPosition = _y;
      updatedHeight = widget.minHeight;
    }
    setState(() {
      _rect = Rect.fromLTWH(
        updatedXPosition,
        updatedYPosition,
        updatedWidth,
        updatedHeight,
      );
    });
  }

  @override
  Widget build(BuildContext context) {
    return Positioned(
      top: _rect.top,
      left: _rect.left,
      child: UnconstrainedBox(
        child: SizedBox(
          width: _rect.width,
          height: _rect.height,
          child: ScalableItem(
            cornerDotColor: Colors.white,
            defaultScaleBorderDecoration: BoxDecoration(
              border: Border.all(width: 8, color: Colors.yellow),
            ),
            showCornerDots: true,
            isScaling: true,
            includedScaleDirections: const [
              ScaleDirection.topLeft,
              ScaleDirection.topCenter,
              ScaleDirection.topRight,
              ScaleDirection.centerRight,
              ScaleDirection.bottomRight,
              ScaleDirection.bottomCenter,
              ScaleDirection.bottomLeft,
              ScaleDirection.centerLeft,
            ],
            onAnyDotDraggingEnd: (details) {
              widget.onScalingEnd(_rect);
              _rect = Rect.fromLTWH(_initX, _initY, _rect.width, _rect.height);
            },
            onTopLeftDotDragging: (details) {
              _onScaling(details, ScaleDirection.topLeft);
            },
            onTopCenterDotDragging: (details) {
              _onScaling(details, ScaleDirection.topCenter);
            },
            onTopRightDotDragging: (details) {
              _onScaling(details, ScaleDirection.topRight);
            },
            onBottomLeftDotDragging: (details) {
              _onScaling(details, ScaleDirection.bottomLeft);
            },
            onBottomCenterDotDragging: (details) {
              _onScaling(details, ScaleDirection.bottomCenter);
            },
            onBottomRightDotDragging: (details) {
              _onScaling(details, ScaleDirection.bottomRight);
            },
            onCenterLeftDotDragging: (details) {
              _onScaling(details, ScaleDirection.centerLeft);
            },
            onCenterRightDotDragging: (details) {
              _onScaling(details, ScaleDirection.centerRight);
            },
            child: const SizedBox(),
          ),
        ),
      ),
    );
  }
}
