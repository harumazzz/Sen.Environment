//import 'dart:math';

//import 'package:collection/collection.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

class Selection {
  Selection({
    this.erased = false,
    this.moved = false,
    this.rectangleSelected = false,
  });

  bool erased;
  bool moved;
  bool rectangleSelected;
}

class CanvasController extends ChangeNotifier {
  CanvasController({required this.transformationController});

  TransformationController transformationController;

  final focusNode = FocusNode();

  double minScale = 0.4;
  double maxScale = 4;

  Selection selection = Selection();

  PointerHoverEvent? _pointerHoverCatched;

  PointerHoverEvent? _pointerHoverEvent;

  Matrix4? _matrix4Catched;

  Matrix4 get matrix => transformationController.value;
  Offset mousePosition = Offset.zero;
  Offset? mouseDragStart;
  Offset? marqueeStart, marqueeEnd;
  Rect? marqueeRect;

  bool _mouseDown = false;
  bool get mouseDown => _mouseDown;
  set mouseDown(bool value) {
    if (value == _mouseDown) {
      return;
    }
    _mouseDown = value;
    notifyListeners();
  }

  bool _shiftPressed = false;
  bool get shiftPressed => _shiftPressed;
  set shiftPressed(bool value) {
    if (value == _shiftPressed) {
      return;
    }
    _shiftPressed = value;
    notifyListeners();
  }

  bool _spacePressed = false;
  bool get spacePressed => _spacePressed;
  set spacePressed(bool value) {
    if (value == _spacePressed) {
      return;
    }
    _spacePressed = value;
    notifyListeners();
  }

  bool _controlPressed = false;
  bool get controlPressed => _controlPressed;
  set controlPressed(bool value) {
    if (value == _controlPressed) {
      return;
    }
    _controlPressed = value;
    notifyListeners();
  }

  bool _metaPressed = false;
  bool get metaPressed => _metaPressed;
  set metaPressed(bool value) {
    if (value == _metaPressed) {
      return;
    }
    _metaPressed = value;
    notifyListeners();
  }

  double _scale = 1;
  double get scale => _scale;
  set scale(double value) {
    if (value == _scale) {
      return;
    }
    _scale = value;
    notifyListeners();
  }

  Offset toLocal(Offset global) {
    return transformationController.toScene(global);
  }

  Rect checkMarqueeSelection() {
    if (marqueeStart == null || marqueeEnd == null) {
      return Rect.zero;
    }
    return Rect.fromPoints(toLocal(marqueeStart!), toLocal(marqueeEnd!));
  }

  void requestFocus() {
    if (!focusNode.hasFocus) {
      focusNode.requestFocus();
    }
  }

  PointerHoverEvent get pointerHoverEvent => _pointerHoverEvent!;

  set pointerHoverEvent(PointerHoverEvent details) {
    _pointerHoverEvent = details;
  }

  void catchPointerHover() {
    _pointerHoverCatched = _pointerHoverEvent;
    _matrix4Catched = transformationController.value.clone();
  }

  Offset getDetailsMove() {
    if (_pointerHoverCatched != null &&
        _pointerHoverEvent != null &&
        _matrix4Catched != null) {
      final viewscalePort = matrix.getMaxScaleOnAxis();
      final dx =
          _pointerHoverEvent!.position.dx - _pointerHoverCatched!.position.dx;
      final dy =
          _pointerHoverEvent!.position.dy - _pointerHoverCatched!.position.dy;
      final viewDx = matrix[12] - _matrix4Catched![12];
      final viewDy = matrix[13] - _matrix4Catched![13];
      return Offset(
        (dx - viewDx) / viewscalePort,
        (dy - viewDy) / viewscalePort,
      );
    } else {
      return Offset.zero;
    }
  }

  void setMarqueeSelection() {
    marqueeRect = checkMarqueeSelection();
  }

  double getScale() {
    final matrix = transformationController.value;
    final scaleX = matrix.getMaxScaleOnAxis();
    return scaleX;
  }

  void reset() {}
}
