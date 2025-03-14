import 'package:flutter/material.dart';

class ItemProfile {
  ItemProfile({
    required this.isEvent,
    this.matrix,
    this.itemRect,
    this.selectRect,
    this.widget,
  });
  final bool isEvent;
  Matrix4? matrix;
  Rect? itemRect;
  Rect? selectRect;
  Widget? widget;
}
