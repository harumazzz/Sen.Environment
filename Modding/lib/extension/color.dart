import 'package:flutter/material.dart';

extension ColorExtension on Color {
  Color blendWith(Color other, double amount) {
    return Color.lerp(this, other, amount) ?? this;
  }
}
