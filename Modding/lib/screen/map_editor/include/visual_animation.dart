import 'dart:async';
import 'dart:ui' as ui;
import 'package:flutter/material.dart';
import '../../../model/animation.dart' as model;
import 'visual_image.dart';
import '../../../service/file_helper.dart';
import 'dart:math' as math;

class VisualAnimation {
  late final Size visualSize;

  late final int frameRate;

  final List<Widget> spriteFrame = [];

  final Map<String, LabelInfo> labelInfo = {};

  final List<VisualSpriteFrame> _spriteList = [];

  final List<(VisualImage, Matrix4)?> _imageList = [];

  final List<int> _spriteDisable = [];

  FilterQuality filterQuality = FilterQuality.high;

  String labelPlay = 'main';

  static Future<VisualAnimation> create(
    String animationPath,
    String mediaPath, {
    FilterQuality? filterQuality,
    Iterable<String>? spriteDisable,
  }) async {
    final component = VisualAnimation();
    if (filterQuality != null) {
      component.filterQuality = filterQuality;
    }
    await component.load(
      animationPath,
      mediaPath,
      spriteDisable: spriteDisable,
    );
    component.initSprite();
    return component;
  }

  static Future<VisualAnimation> createByAnimation(
    model.SexyAnimation animation,
    String mediaPath, {
    FilterQuality? filterQuality,
    Iterable<String>? spriteDisable,
  }) async {
    final component = VisualAnimation();
    if (filterQuality != null) {
      component.filterQuality = filterQuality;
    }
    await component.process(animation, mediaPath, spriteDisable: spriteDisable);
    component.initSprite();
    return component;
  }

  Future<void> load(
    String animationPath,
    String mediaPath, {
    Iterable<String>? spriteDisable,
  }) async {
    final animation = model.SexyAnimation.fromJson(
      await FileHelper.readJsonAsync(source: animationPath),
    );
    await process(animation, mediaPath, spriteDisable: spriteDisable);
    return;
  }

  Future<void> process(
    model.SexyAnimation animation,
    String mediaPath, {
    Iterable<String>? spriteDisable,
  }) async {
    clear();
    visualSize = Size(animation.size.width, animation.size.height);
    frameRate = animation.frameRate;
    for (final image in animation.image) {
      await _loadImage(image, mediaPath);
    }
    var index = 0;
    for (final sprite in animation.sprite) {
      _loadSprite(sprite);
      if (spriteDisable != null && (spriteDisable.contains(sprite.name))) {
        _spriteDisable.add(index);
      }
      ++index;
    }
    _loadSprite(animation.mainSprite);
    _loadLabel(animation.mainSprite);
  }

  void _drawWidget(SpritePainter children, VisualLayer layer) {
    if (layer.spriteFrame != null) {
      if (_spriteDisable.contains(layer.resource)) {
        return;
      }
      final sprite = _spriteList[layer.resource];
      var frameIndex = layer.spriteFrame!;
      while (frameIndex >= sprite.length) {
        frameIndex -= sprite.length;
      }
      final spriteFrame = sprite[frameIndex];
      for (final layerIndex in spriteFrame.keys) {
        final visualLayer = spriteFrame[layerIndex]!.clone();
        visualLayer.matrix = layer.matrix.multiplied(visualLayer.matrix);
        visualLayer.color = VisualFrame.mixColor(
          visualLayer.color,
          layer.color,
        );
        _drawWidget(children, visualLayer);
      }
    } else {
      final matrix = layer.matrix.multiplied(_imageList[layer.resource]!.$2);
      final color = ui.ColorFilter.mode(layer.color, ui.BlendMode.modulate);
      children.add(_imageList[layer.resource]!.$1, matrix, color);
    }
  }

  void initSprite({int? index}) {
    spriteFrame.clear();
    spriteFrame.addAll(selectSprite(index: index));
  }

  List<Widget> selectSprite({int? index}) {
    VisualSpriteFrame sprite;
    if (index != null && _spriteList.elementAtOrNull(index) == null) {
      sprite = _spriteList[index];
    }
    sprite = _spriteList.last; // mainsprite
    final List<Widget> frameWidget = [];
    for (var i = 0; i < sprite.length; ++i) {
      final layerList = sprite[i];
      final keys = layerList.keys.toList();
      keys.sort();
      final spritePainter = SpritePainter(filterQuality: filterQuality);
      for (final layerIndex in keys) {
        _drawWidget(spritePainter, layerList[layerIndex]!);
      }
      frameWidget.add(CustomPaint(painter: spritePainter));
    }
    return frameWidget;
  }

  void _loadLabel(model.AnimationSprite sprite) {
    var label = 'main';
    var frameIndex = 0;
    for (final frame in sprite.frame) {
      if (frame.label != '') {
        label = frame.label;
        labelInfo[label] = LabelInfo(begin: frameIndex, end: frameIndex);
      }
      ++labelInfo[label]!.end;
      ++frameIndex;
    }
    if (!labelInfo.containsKey('main')) {
      labelInfo['main'] = LabelInfo(begin: 0, end: frameIndex);
    }
  }

  void _loadSprite(model.AnimationSprite sprite) {
    _spriteList.add(VisualFrame.visualizeSprite(sprite));
  }

  Future<void> _loadImage(model.AnimationImage image, String mediaPath) async {
    final imagePath = '$mediaPath/${image.path}.png';
    if (!FileHelper.isFile(imagePath)) {
      _imageList.add(null);
      return;
    }
    final completer = Completer<VisualImage>();
    final listener = ImageStreamListener((info, _) {
      completer.complete(info.image);
    });

    final stream = ResizeImage(
      MemoryImage(await FileHelper.readBufferAsync(source: imagePath)),
      width: image.dimension.width,
      height: image.dimension.height,
    ).resolve(const ImageConfiguration())..addListener(listener);
    final data = (
      await completer.future,
      VisualFrame.transformMatrixFromVariant(image.transform),
    );
    _imageList.add(data);
    stream.removeListener(listener);
    return;
  }

  void clear() {
    spriteFrame.clear();
    labelInfo.clear();
    _spriteList.clear();
    _imageList.clear();
  }
}

typedef LayerFrame = Map<int, VisualLayer>;

typedef VisualSpriteFrame = List<LayerFrame>;

LayerFrame cloneLayerFrame(LayerFrame original) {
  final LayerFrame copy = {};
  original.forEach((key, value) {
    copy[key] = value.clone();
  });
  return copy;
}

final Matrix4 initialMatrix = Matrix4.identity();

const Color initialColor = Colors.white;

class VisualFrame {
  static Matrix4 transformMatrixFromVariant(model.Transform transform) {
    final matrix = Matrix4.identity();
    switch (transform.length) {
      case 2:
        {
          matrix[12] = transform[0];
          matrix[13] = transform[1];
          break;
        }
      case 3:
        {
          final cos = math.cos(transform[0]);
          final sin = math.sin(transform[0]);
          matrix[0] = cos;
          matrix[1] = sin;
          matrix[4] = -sin;
          matrix[5] = cos;
          matrix[12] = transform[1];
          matrix[13] = transform[2];
          break;
        }
      case 6:
        {
          matrix[0] = transform[0];
          matrix[1] = transform[1];
          matrix[4] = transform[2];
          matrix[5] = transform[3];
          matrix[12] = transform[4];
          matrix[13] = transform[5];
          break;
        }
      default:
        break;
    }
    return matrix;
  }

  static Color colorFromVariant(model.Color color) {
    return Color.fromRGBO(
      (color[0] * 255).round(),
      (color[1] * 255).round(),
      (color[2] * 255).round(),
      color[3],
    );
  }

  static Color mixColor(Color source, Color change) {
    return Color.from(
      red: source.r * change.r,
      green: source.g * change.g,
      blue: source.b * change.b,
      alpha: source.a * change.a,
    );
  }

  static VisualSpriteFrame visualizeSprite(model.AnimationSprite sprite) {
    late VisualSpriteFrame visualFrameSprite = [];
    late LayerFrame layerList = {};
    for (final frame in sprite.frame) {
      frame.remove.forEach(layerList.remove);
      for (final append in frame.append) {
        final layer = VisualLayer(
          resource: append.resource,
          matrix: initialMatrix,
          color: initialColor,
        );
        layer.spriteFrame = append.sprite ? 0 : null;
        layerList[append.index] = layer;
      }
      for (final layerIndex in layerList.keys) {
        final layer = layerList[layerIndex]!;
        if (layer.spriteFrame != null) {
          layer.spriteFrame = layer.spriteFrame! + 1;
        }
      }
      for (final change in frame.change) {
        if (!layerList.containsKey(change.index)) {
          continue;
        }
        final layer = layerList[change.index]!;
        if (layer.spriteFrame != null) {
          layer.spriteFrame = 0;
        }
        layer.matrix = transformMatrixFromVariant(change.transform);
        if (change.color != null) {
          layer.color = colorFromVariant(change.color!);
        }
      }
      visualFrameSprite.add(cloneLayerFrame(layerList));
    }
    return visualFrameSprite;
  }
}

class LabelInfo {
  LabelInfo({required this.begin, required this.end});
  final int begin;
  int end;
}

class VisualLayer {
  VisualLayer({
    required this.resource,
    required this.matrix,
    required this.color,
    this.spriteFrame,
  });
  int resource;
  Matrix4 matrix;
  Color color;
  int? spriteFrame;

  VisualLayer clone() {
    return VisualLayer(
      resource: resource,
      matrix: Matrix4.copy(matrix),
      color: color,
      spriteFrame: spriteFrame,
    );
  }
}

class SpritePainter extends CustomPainter {
  SpritePainter({required this.filterQuality});

  final FilterQuality filterQuality;

  final List<(VisualImage?, Matrix4?, ColorFilter?)> _list = [];

  void add(VisualImage? image, Matrix4? matrix, ColorFilter? color) {
    _list.add((image, matrix, color));
  }

  @override
  void paint(Canvas canvas, Size size) {
    for (var i = 0; i < _list.length; ++i) {
      final image = _list[i].$1;
      if (image == null) {
        continue;
      }
      final matrix = _list[i].$2;
      canvas.save();
      if (matrix != null) {
        canvas.transform(matrix.storage);
      }
      final paint = Paint()..filterQuality = filterQuality;
      final color = _list[i].$3;
      if (color != null) {
        paint.colorFilter = color;
      }
      canvas.drawImage(image, Offset.zero, paint);
      canvas.restore();
    }
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) {
    return true;
  }
}
