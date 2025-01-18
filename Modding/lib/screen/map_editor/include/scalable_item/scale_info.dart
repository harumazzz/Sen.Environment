import 'package:flutter/material.dart';
import 'package:sen/screen/map_editor/include/scalable_item/scale_direction_enum.dart';

enum Shape { circle, oval, rectangle }

/// Contain all things that can affect scaling logic
@immutable
class ScaleInfoOpt {
  final Shape shape;
  final ScaleDirection scaleDirection;
  final double dx;
  final double dy;
  final double rotateAngle;

  const ScaleInfoOpt({
    required this.shape,
    required this.scaleDirection,
    required this.dx,
    required this.dy,
    required this.rotateAngle,
  });
}

/// Contain all things for scaling in UI.
@immutable
class ScaleInfo {
  final double width;
  final double height;
  final double x;
  final double y;

  const ScaleInfo({
    required this.width,
    required this.height,
    required this.x,
    required this.y,
  });

  ScaleInfo copyWith({
    double? width,
    double? height,
    double? x,
    double? y,
  }) {
    return ScaleInfo(
      width: width ?? this.width,
      height: height ?? this.height,
      x: x ?? this.x,
      y: y ?? this.y,
    );
  }

  Map<String, dynamic> toMap() {
    Map<String, dynamic> map = {};
    map["width"] = width;
    map["height"] = height;
    map["x"] = x;
    map["y"] = y;

    return map;
  }
}
