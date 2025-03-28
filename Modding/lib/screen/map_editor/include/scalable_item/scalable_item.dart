import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'scale_direction_enum.dart';

const defaultDotColor = Colors.blue;
const defaultOverscaleDotColor = Colors.redAccent;
const defaultScaleBorderWidth = 5.0;

class ScalableItem extends StatelessWidget {
  const ScalableItem({
    super.key,
    required this.child,
    required this.showCornerDots,
    required this.includedScaleDirections,
    required this.isScaling,
    this.showOverScaleBorder = false,
    this.onBottomLeftDotDragging,
    this.onBottomCenterDotDragging,
    this.onBottomRightDotDragging,
    this.onTopLeftDotDragging,
    this.onTopCenterDotDragging,
    this.onTopRightDotDragging,
    this.onCenterLeftDotDragging,
    this.onCenterRightDotDragging,
    this.cornerDotColor = defaultDotColor,
    this.overScaleCornerDotColor = defaultOverscaleDotColor,
    this.onAnyDotDraggingEnd,
    this.overScaleBorderDecoration,
    this.defaultScaleBorderDecoration,
  });

  final Widget child;
  final Color cornerDotColor;
  final Color overScaleCornerDotColor;
  final bool showCornerDots;
  final bool showOverScaleBorder;
  final Decoration? overScaleBorderDecoration;
  final Decoration? defaultScaleBorderDecoration;
  final Function(DragUpdateDetails)? onTopLeftDotDragging;
  final Function(DragUpdateDetails)? onTopCenterDotDragging;
  final Function(DragUpdateDetails)? onTopRightDotDragging;
  final Function(DragUpdateDetails)? onBottomLeftDotDragging;
  final Function(DragUpdateDetails)? onBottomCenterDotDragging;
  final Function(DragUpdateDetails)? onBottomRightDotDragging;
  final Function(DragUpdateDetails)? onCenterLeftDotDragging;
  final Function(DragUpdateDetails)? onCenterRightDotDragging;
  final Function(DragEndDetails)? onAnyDotDraggingEnd;
  final List<ScaleDirection> includedScaleDirections;
  final bool isScaling;

  @override
  Widget build(BuildContext context) {
    final Color computedDotColor =
        showCornerDots
            ? showOverScaleBorder
                ? overScaleCornerDotColor
                : cornerDotColor
            : Colors.transparent;

    final Decoration localOverScaleBorderDecoration =
        overScaleBorderDecoration ??
        BoxDecoration(
          border: Border.all(
            width: defaultScaleBorderWidth,
            color: computedDotColor,
          ),
        );

    final Decoration localDefaultScaleBorderDecoration =
        defaultScaleBorderDecoration ??
        BoxDecoration(
          border: Border.all(
            width: defaultScaleBorderWidth,
            color: Colors.grey[100]!,
          ),
        );

    final Decoration borderDecoration =
        showOverScaleBorder
            ? localOverScaleBorderDecoration
            : localDefaultScaleBorderDecoration;

    return Stack(
      fit: StackFit.passthrough,
      children: [
        // child
        Positioned.fill(child: child),

        if (isScaling)
          Positioned.fill(child: Container(decoration: borderDecoration)),

        // top left
        if (includedScaleDirections.contains(ScaleDirection.topLeft))
          GestureDetector(
            onPanUpdate: (details) {
              if (onTopLeftDotDragging != null) {
                onTopLeftDotDragging!(details);
              }
            },
            onPanEnd: (details) {
              if (onAnyDotDraggingEnd != null) {
                onAnyDotDraggingEnd!(details);
              }
            },
            child: Container(
              alignment: Alignment.topLeft,
              child: CustomDot(color: computedDotColor),
            ),
          ),

        // top center
        if (includedScaleDirections.contains(ScaleDirection.topCenter))
          GestureDetector(
            onPanUpdate: (details) {
              if (onTopCenterDotDragging != null) {
                onTopCenterDotDragging!(details);
              }
            },
            onPanEnd: (details) {
              if (onAnyDotDraggingEnd != null) {
                onAnyDotDraggingEnd!(details);
              }
            },
            child: Container(
              alignment: Alignment.topCenter,
              child: CustomDot(color: computedDotColor),
            ),
          ),

        // top right
        if (includedScaleDirections.contains(ScaleDirection.topRight))
          GestureDetector(
            onPanUpdate: (details) {
              if (onTopRightDotDragging != null) {
                onTopRightDotDragging!(details);
              }
            },
            onPanEnd: (details) {
              if (onAnyDotDraggingEnd != null) {
                onAnyDotDraggingEnd!(details);
              }
            },
            child: Container(
              alignment: Alignment.topRight,
              child: CustomDot(color: computedDotColor),
            ),
          ),

        // bottom left
        if (includedScaleDirections.contains(ScaleDirection.bottomLeft))
          GestureDetector(
            onPanUpdate: (details) {
              if (onBottomLeftDotDragging != null) {
                onBottomLeftDotDragging!(details);
              }
            },
            onPanEnd: (details) {
              if (onAnyDotDraggingEnd != null) {
                onAnyDotDraggingEnd!(details);
              }
            },
            child: Container(
              alignment: Alignment.bottomLeft,
              child: CustomDot(color: computedDotColor),
            ),
          ),

        // bottom center
        if (includedScaleDirections.contains(ScaleDirection.bottomCenter))
          GestureDetector(
            onPanUpdate: (details) {
              if (onBottomCenterDotDragging != null) {
                onBottomCenterDotDragging!(details);
              }
            },
            onPanEnd: (details) {
              if (onAnyDotDraggingEnd != null) {
                onAnyDotDraggingEnd!(details);
              }
            },
            child: Container(
              alignment: Alignment.bottomCenter,
              child: CustomDot(color: computedDotColor),
            ),
          ),

        // bottom right
        if (includedScaleDirections.contains(ScaleDirection.bottomRight))
          GestureDetector(
            onPanUpdate: (details) {
              if (onBottomRightDotDragging != null) {
                onBottomRightDotDragging!(details);
              }
            },
            onPanEnd: (details) {
              if (onAnyDotDraggingEnd != null) {
                onAnyDotDraggingEnd!(details);
              }
            },
            child: Container(
              alignment: Alignment.bottomRight,
              child: CustomDot(color: computedDotColor),
            ),
          ),

        // center left
        if (includedScaleDirections.contains(ScaleDirection.centerLeft))
          GestureDetector(
            onPanUpdate: (details) {
              if (onCenterLeftDotDragging != null) {
                onCenterLeftDotDragging!(details);
              }
            },
            onPanEnd: (details) {
              if (onAnyDotDraggingEnd != null) {
                onAnyDotDraggingEnd!(details);
              }
            },
            child: Container(
              alignment: Alignment.centerLeft,
              child: CustomDot(color: computedDotColor),
            ),
          ),

        // center right
        if (includedScaleDirections.contains(ScaleDirection.centerRight))
          GestureDetector(
            onPanUpdate: (details) {
              if (onCenterRightDotDragging != null) {
                onCenterRightDotDragging!(details);
              }
            },
            onPanEnd: (details) {
              if (onAnyDotDraggingEnd != null) {
                onAnyDotDraggingEnd!(details);
              }
            },
            child: Container(
              alignment: Alignment.centerRight,
              child: CustomDot(color: computedDotColor),
            ),
          ),
      ],
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(ColorProperty('cornerDotColor', cornerDotColor));
    properties.add(
      ColorProperty('overScaleCornerDotColor', overScaleCornerDotColor),
    );
    properties.add(DiagnosticsProperty<bool>('showCornerDots', showCornerDots));
    properties.add(
      DiagnosticsProperty<bool>('showOverScaleBorder', showOverScaleBorder),
    );
    properties.add(
      DiagnosticsProperty<Decoration?>(
        'overScaleBorderDecoration',
        overScaleBorderDecoration,
      ),
    );
    properties.add(
      DiagnosticsProperty<Decoration?>(
        'defaultScaleBorderDecoration',
        defaultScaleBorderDecoration,
      ),
    );
    properties.add(
      ObjectFlagProperty<Function(DragUpdateDetails p1)?>.has(
        'onTopLeftDotDragging',
        onTopLeftDotDragging,
      ),
    );
    properties.add(
      ObjectFlagProperty<Function(DragUpdateDetails p1)?>.has(
        'onTopCenterDotDragging',
        onTopCenterDotDragging,
      ),
    );
    properties.add(
      ObjectFlagProperty<Function(DragUpdateDetails p1)?>.has(
        'onTopRightDotDragging',
        onTopRightDotDragging,
      ),
    );
    properties.add(
      ObjectFlagProperty<Function(DragUpdateDetails p1)?>.has(
        'onBottomLeftDotDragging',
        onBottomLeftDotDragging,
      ),
    );
    properties.add(
      ObjectFlagProperty<Function(DragUpdateDetails p1)?>.has(
        'onBottomCenterDotDragging',
        onBottomCenterDotDragging,
      ),
    );
    properties.add(
      ObjectFlagProperty<Function(DragUpdateDetails p1)?>.has(
        'onBottomRightDotDragging',
        onBottomRightDotDragging,
      ),
    );
    properties.add(
      ObjectFlagProperty<Function(DragUpdateDetails p1)?>.has(
        'onCenterLeftDotDragging',
        onCenterLeftDotDragging,
      ),
    );
    properties.add(
      ObjectFlagProperty<Function(DragUpdateDetails p1)?>.has(
        'onCenterRightDotDragging',
        onCenterRightDotDragging,
      ),
    );
    properties.add(
      ObjectFlagProperty<Function(DragEndDetails p1)?>.has(
        'onAnyDotDraggingEnd',
        onAnyDotDraggingEnd,
      ),
    );
    properties.add(
      IterableProperty<ScaleDirection>(
        'includedScaleDirections',
        includedScaleDirections,
      ),
    );
    properties.add(DiagnosticsProperty<bool>('isScaling', isScaling));
  }
}

class CustomDot extends StatelessWidget {
  const CustomDot({super.key, required this.color});

  final Color color;

  @override
  Widget build(BuildContext context) {
    return Icon(Symbols.square, size: 25, color: color);
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(ColorProperty('color', color));
  }
}
