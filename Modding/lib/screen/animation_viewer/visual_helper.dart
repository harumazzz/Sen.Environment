import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../bloc/selected_image_bloc/selected_image_bloc.dart';
import '../../bloc/selected_label_bloc/selected_label_bloc.dart';
import '../../bloc/selected_sprite_bloc/selected_sprite_bloc.dart';
import '../../model/animation.dart' as model;
import 'label_info.dart';
import '../../service/file_helper.dart';
import 'dart:collection';
import 'dart:math' as math;

class VisualHelper {
  VisualHelper({
    Matrix4? initialMatrix,
    model.Color? initialColor,
    List<ImageProvider?>? imageSource,
    bool? hasAnimation,
    bool? hasMedia,
    int? workingSpriteIndex,
    double? workingFrameRate,
    required this.context,
    Map<String, LabelInfo>? labelInfo,
  }) : initialMatrix = initialMatrix ?? Matrix4.identity(),
       initialColor = initialColor ?? [1.0, 1.0, 1.0, 1.0],
       imageSource = imageSource ?? [],
       hasAnimation = hasAnimation ?? false,
       hasMedia = hasMedia ?? false,
       workingSpriteIndex = workingSpriteIndex ?? 0,
       workingFrameRate = workingFrameRate ?? 30,
       labelInfo = labelInfo ?? {};
  Matrix4 initialMatrix;
  model.Color initialColor;
  late model.SexyAnimation animation;
  List<ImageProvider?> imageSource;
  bool hasAnimation;
  bool hasMedia;
  int workingSpriteIndex;
  double workingFrameRate;
  Map<String, LabelInfo> labelInfo;
  final BuildContext context;

  Future<void> loadAnimation(String path) async {
    dispose();
    animation = model.SexyAnimation.fromJson(
      await FileHelper.readJsonAsync(source: path),
    );
    return;
  }

  Future<void> loadImageSource(String directory) async {
    for (final image in animation.image) {
      final file = '$directory/${image.path}.png';
      var source = null as MemoryImage?;
      if (FileHelper.isFile(file)) {
        source = MemoryImage(await FileHelper.readBufferAsync(source: file));
      }
      imageSource.add(source);
    }
    return;
  }

  Matrix4 transformMatrixFromVariant(model.Transform transform) {
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

  ColorFilter makeColor(model.Color value) {
    final result = ColorFilter.mode(
      Color.fromARGB(
        (255.0 * value[3]).toInt(),
        (255.0 * value[0]).toInt(),
        (255.0 * value[1]).toInt(),
        (255.0 * value[2]).toInt(),
      ),
      BlendMode.modulate,
    );
    return result;
  }

  Color colorFromVariant(model.Color color) {
    return Color.fromRGBO(
      (color[0] * 255).round(),
      (color[1] * 255).round(),
      (color[2] * 255).round(),
      color[3],
    );
  }

  model.AnimationImage selectImage(int index) {
    var result = null as model.AnimationImage?;
    if (0 <= index && index < animation.image.length) {
      result = animation.image[index];
    } else {
      throw Exception();
    }
    return result;
  }

  model.AnimationSprite selectSprite(int index) {
    var result = null as model.AnimationSprite?;
    if (0 <= index && index < animation.sprite.length) {
      result = animation.sprite[index];
    } else if (index == animation.sprite.length) {
      result = animation.mainSprite;
    } else {
      throw Exception();
    }
    return result;
  }

  bool containSprite(int index) {
    if (0 <= index && index <= animation.sprite.length) {
      return true;
    } else {
      return false;
    }
  }

  Widget visualizeImage(int index) {
    final image = selectImage(index);
    return Visibility(
      visible: context.read<SelectedImageBloc>().state.value[index],
      child: Transform(
        transform: transformMatrixFromVariant(image.transform),
        child:
            imageSource[index] == null
                ? Text('Missing ${image.path}')
                : Image(
                  image: imageSource[index]!,
                  width: image.dimension.width.toDouble(),
                  height: image.dimension.height.toDouble(),
                  fit: BoxFit.fill,
                ),
      ),
    );
  }

  Widget visualizeSprite(int index, AnimationController animationController) {
    final sprite = selectSprite(index);
    final isMainSprite = index == animation.sprite.length;
    final layerList = SplayTreeMap<int, _VisualLayer>();
    var frameIndex = 0;
    for (final frame in sprite.frame) {
      for (final removeIndex in frame.remove) {
        final layer = layerList[removeIndex]!;
        layer.isRemoved = true;
      }
      for (final action in frame.append) {
        if (layerList.containsKey(action.index)) {
          throw Exception();
        }
        final currentLabel = context.read<SelectedLabelBloc>().state.label;
        final layer = layerList[action.index] = _VisualLayer();
        final subController =
            isMainSprite
                ? animationController.drive(
                  IntTween(
                    begin: labelInfo[currentLabel]!.startIndex,
                    end: labelInfo[currentLabel]!.endIndex - 1,
                  ),
                )
                : animationController.drive(
                  IntTween(begin: 0, end: sprite.frame.length - 1),
                );
        layer.view = AnimatedBuilder(
          animation: subController,
          child:
              !action.sprite
                  ? visualizeImage(action.resource)
                  : visualizeSprite(action.resource, animationController),
          builder: (context, child) {
            var index = subController.value;
            var property = layer.property[index];
            if (property == null) {
              return const SizedBox();
            }
            return ColorFiltered(
              colorFilter: property.$2,
              child: Transform(transform: property.$1, child: child),
            );
          },
        );
        layer.property = List.filled(sprite.frame.length, null);
        layer.property[frameIndex] = (initialMatrix, makeColor(initialColor));
        layer.isRemoved = false;
        layer.isChanged = true;
      }
      for (final action in frame.change) {
        final layer = layerList[action.index]!;
        if (layer.isChanged) {
          layer.property[frameIndex] = (
            transformMatrixFromVariant(action.transform),
            action.color != null
                ? makeColor(action.color!)
                : layer.property[frameIndex]!.$2,
          );
        } else {
          layer.property[frameIndex] = (
            transformMatrixFromVariant(action.transform),
            action.color != null
                ? makeColor(action.color!)
                : layer.property[frameIndex - 1]!.$2,
          );
        }
        layer.isChanged = true;
      }
      for (final layer in layerList.values) {
        if (layer.isRemoved) {
          continue;
        }
        if (layer.isChanged) {
          layer.isChanged = false;
          continue;
        }
        layer.property[frameIndex] = layer.property[frameIndex - 1];
      }
      ++frameIndex;
    }

    if (isMainSprite) {
      return Stack(
        fit: StackFit.passthrough,
        children: layerList.values.map((value) => value.view).toList(),
      );
    } else {
      return Visibility(
        visible: context.read<SelectedSpriteBloc>().state.value[index],
        child: Stack(
          fit: StackFit.passthrough,
          children: layerList.values.map((value) => value.view).toList(),
        ),
      );
    }
  }

  void dispose() {
    hasAnimation = false;
    hasMedia = false;
    workingFrameRate = 30;
    imageSource = [];
    labelInfo = {};
  }
}

class _VisualLayer {
  late Widget view;
  late List<(Matrix4, ColorFilter)?> property;
  late bool isRemoved;
  late bool isChanged;
}
