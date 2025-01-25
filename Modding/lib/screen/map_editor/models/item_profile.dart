import 'package:flutter/material.dart';

class ItemProfile {
  final bool isEvent;
  Matrix4? matrix;
  Rect? itemRect;
  Rect? selectRect;
  Widget? widget;

  ItemProfile({
    required this.isEvent,
    this.matrix,
    this.itemRect,
    this.selectRect,
    this.widget,
  });
}
