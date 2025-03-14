// ignore_for_file: public_member_api_docs, sort_constructors_first
import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';

import 'scale_direction_enum.dart';

enum Shape { circle, oval, rectangle }

/// Contain all things that can affect scaling logic
@immutable
class ScaleInfoOpt extends Equatable {
  const ScaleInfoOpt({
    required this.shape,
    required this.scaleDirection,
    required this.dx,
    required this.dy,
    required this.rotateAngle,
  });
  final Shape shape;
  final ScaleDirection scaleDirection;
  final double dx;
  final double dy;
  final double rotateAngle;

  @override
  List<Object> get props {
    return [shape, scaleDirection, dx, dy, rotateAngle];
  }
}

/// Contain all things for scaling in UI.
@immutable
class ScaleInfo extends Equatable {
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

  ScaleInfo copyWith({double? width, double? height, double? x, double? y}) {
    return ScaleInfo(
      width: width ?? this.width,
      height: height ?? this.height,
      x: x ?? this.x,
      y: y ?? this.y,
    );
  }

  Map<String, dynamic> toMap() {
    final map = <String, dynamic>{};
    map['width'] = width;
    map['height'] = height;
    map['x'] = x;
    map['y'] = y;

    return map;
  }

  @override
  List<Object> get props => [width, height, x, y];
}
