import 'dart:io';

import 'package:desktop_drop/desktop_drop.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:screenshot/screenshot.dart';
import 'package:sen/bloc/selected_image_bloc/selected_image_bloc.dart';
import 'package:sen/bloc/selected_label_bloc/selected_label_bloc.dart';
import 'package:sen/bloc/selected_sprite_bloc/selected_sprite_bloc.dart';
import 'package:sen/screen/animation_viewer/control_panel.dart';
import 'package:sen/screen/animation_viewer/label_screen.dart';
import 'package:sen/screen/animation_viewer/media_screen.dart';
import 'package:sen/screen/animation_viewer/visual_helper.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class AnimationScreen extends StatefulWidget {
  const AnimationScreen({
    super.key,
    required this.hasFile,
    required this.onUploadFile,
    required this.onDragFile,
    required this.mediaScreen,
    required this.labelScreen,
    required this.animationController,
    required this.visualHelper,
    required this.selectedLabelBloc,
    required this.controller,
  });

  final bool hasFile;

  final void Function() onUploadFile;

  final void Function(String value) onDragFile;

  final MediaScreen mediaScreen;

  final LabelScreen labelScreen;

  final AnimationController animationController;

  final VisualHelper visualHelper;

  final SelectedLabelBloc selectedLabelBloc;

  final ScreenshotController controller;

  @override
  State<AnimationScreen> createState() => _AnimationScreenState();
}

class _AnimationScreenState extends State<AnimationScreen> {
  // late ValueNotifier<double> _xOffsetNotifier;
  // late ValueNotifier<double> _yOffsetNotifier;
  bool _dragging = false;
  late ValueNotifier<double> _scaleNotifier;
  late ValueNotifier<double> _frameNotifier;
  late TransformationController _transformationController;

  late Widget? _animationVisual;
  bool _isPause = false;

  double _maxFrame = 0;

  @override
  void initState() {
    _scaleNotifier = ValueNotifier(1.0);
    // _xOffsetNotifier = ValueNotifier(0.0);
    // _yOffsetNotifier = ValueNotifier(0.0);
    _frameNotifier = ValueNotifier(0.0);
    super.initState();
    _animationVisual = null;
    _transformationController = TransformationController();
  }

  @override
  void dispose() {
    _transformationController.dispose();
    super.dispose();
    return;
  }

  Widget _uploadFile() {
    final los = AppLocalizations.of(context)!;
    return InkWell(
      splashColor: Colors.blue.withAlpha(30),
      onTap: widget.onUploadFile,
      child: Center(
        child: _dragging
            ? Text(los.drop_file_to_upload)
            : Text(los.upload_file_to_continue),
      ),
    );
  }

  Widget _dragAndDropWidget(Widget data) {
    if (Platform.isAndroid || Platform.isIOS) {
      return data;
    }
    return DropTarget(
      onDragEntered: (details) {
        setState(() {
          _dragging = true;
        });
      },
      onDragExited: (details) {
        setState(() {
          _dragging = false;
        });
      },
      onDragDone: (details) {
        if (details.files.isNotEmpty) {
          final file = details.files.first;
          widget.onDragFile(file.path);
          if (widget.visualHelper.hasAnimation) {
            widget.visualHelper.workingFrameRate =
                widget.visualHelper.animation.frameRate.toDouble();
          }
        }
      },
      child: data,
    );
  }

  void _loadWorkingSprite(int index) {
    final labelInfo =
        widget.visualHelper.labelInfo[widget.selectedLabelBloc.state.label]!;
    final duration = ((labelInfo.endIndex - labelInfo.startIndex) /
            widget.visualHelper.workingFrameRate *
            1000)
        .toInt();
    widget.animationController.duration = Duration(milliseconds: duration);
    _animationVisual =
        widget.visualHelper.visualizeSprite(index, widget.animationController);
    _animationVisual = SizedBox.fromSize(
      size: Size(widget.visualHelper.animation.size.width,
          widget.visualHelper.animation.size.height),
      child: UnconstrainedBox(
        child: SizedOverflowBox(
          alignment: AlignmentDirectional.topStart,
          size: Size(widget.visualHelper.animation.size.width,
              widget.visualHelper.animation.size.height),
          child: _animationVisual,
        ),
      ),
    );
    _maxFrame = (labelInfo.endIndex - labelInfo.startIndex).toDouble();
    widget.animationController.addListener(() {
      _frameNotifier.value = widget.animationController.value;
    });
    if (!_isPause) {
      widget.animationController.repeat();
    }
    final matrix = _transformationController.value;
    matrix[0] = 2;
    matrix[1] = 0;
    matrix[4] = 0;
    matrix[5] = 2;
    matrix[12] = -widget.visualHelper.animation.size.width * 4;
    matrix[13] = -widget.visualHelper.animation.size.height * 4;
    _transformationController.value = matrix;
  }

  /*

  Widget _buildMainContainer({
    required double scale,
    required double xOffset,
    required double yOffset,
  }) {
    return Builder(builder: (context) {
      return Screenshot(
        controller: widget.controller,
        child: ClipRRect(
          borderRadius: BorderRadius.circular(20),
          child: Transform(
            transform: widget.visualHelper.transformMatrixFromVariant([
              xOffset,
              yOffset,
            ]),
            child: Transform.scale(
              scale: scale,
              child: ConstrainedBox(
                constraints: BoxConstraints(
                  maxWidth: MediaQuery.of(context).size.width * 0.6,
                  maxHeight: MediaQuery.of(context).size.height * 0.6,
                ),
                child: FittedBox(
                  fit: BoxFit.contain,
                  child: _animationVisual,
                ),
              ),
            ),
          ),
        ),
      );
    });
  }
  */

  /*
  Widget _buildAnimationVisual() {
    return ValueListenableBuilder<double>(
      valueListenable: _yOffsetNotifier,
      builder: (context, yOffset, child) {
        return ValueListenableBuilder<double>(
          valueListenable: _xOffsetNotifier,
          builder: (context, xOffset, child) => ValueListenableBuilder<double>(
            valueListenable: _scaleNotifier,
            builder: (context, scale, child) {
              return _buildMainContainer(
                  scale: scale, xOffset: xOffset, yOffset: yOffset);
            },
          ),
        );
      },
    );
  }
  */

  Widget _buildMainAnimationScreen() {
    return Screenshot(
        controller: widget.controller,
        child:  ClipRRect(
      borderRadius: BorderRadius.circular(20),
      child: InteractiveViewer.builder(
          panEnabled: true,
          scaleEnabled: true,
          scaleFactor: 1000,
          maxScale: 10,
          transformationController: _transformationController,
          builder: (context, quad) {
            return Transform.scale(
              scale: 0.5,
              child: SizedBox(
                  width: widget.visualHelper.animation.size.width * 5,
                  height: widget.visualHelper.animation.size.height * 5,
                  child: _animationVisual),
            );
          }),
    ));
  }

  Widget _painterOrUpload() {
    if (widget.visualHelper.hasAnimation && widget.visualHelper.hasMedia) {
      _loadWorkingSprite(widget.visualHelper.animation.sprite.length);
      return _buildMainAnimationScreen();
    } else {
      return ClipRRect(
        clipBehavior: Clip.hardEdge,
        child: _dragAndDropWidget(_uploadFile()),
      );
    }
  }

  Widget _buildFrameSilder({
    required String label,
    required ValueNotifier<double> notifier,
    required double min,
    required double max,
  }) {
    return ValueListenableBuilder<double>(
      valueListenable: notifier,
      builder: (context, value, child) {
        return Card(
          child: Row(
            children: [
              const SizedBox(width: 16),
              Text(
                label,
                style: Theme.of(context)
                    .textTheme
                    .titleSmall!
                    .copyWith(fontWeight: FontWeight.bold),
              ),
              const SizedBox(width: 8.0),
              Expanded(
                child: Tooltip(
                  message: (value * _maxFrame).toStringAsFixed(0),
                  showDuration: Duration.zero,
                  child: Slider(
                    value: value,
                    min: min,
                    max: max,
                    onChanged: (val) {
                      if (_maxFrame > 0) {
                        widget.animationController
                            .animateTo(val, duration: Duration.zero);
                        if (!_isPause) {
                          widget.animationController.repeat();
                        }
                      }
                    },
                  ),
                ),
              ),
            ],
          ),
        );
      },
    );
  }

  Widget _buildContainer() {
    return Expanded(
      child: Container(
        width: double.infinity,
        margin: const EdgeInsets.symmetric(horizontal: 16.0),
        decoration: BoxDecoration(
          borderRadius: BorderRadius.circular(20),
          border: Border.all(
            color: Theme.of(context).brightness == Brightness.dark
                ? Colors.white.withValues(alpha: 0.3)
                : Colors.black.withValues(alpha: 0.3),
            width: 2,
          ),
        ),
        child: _painterOrUpload(),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    context.watch<SelectedLabelBloc>();
    context.watch<SelectedImageBloc>();
    context.watch<SelectedSpriteBloc>();
    final los = AppLocalizations.of(context)!;
    return Column(
      children: [
        _buildContainer(),
        Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              _buildFrameSilder(
                label: "Frame", //TODO:
                notifier: _frameNotifier,
                min: widget.animationController.lowerBound,
                max: widget.animationController.upperBound,
              )
            ],
          ),
        ),
        ControlPanel(
          visualHelper: widget.visualHelper,
          animationController: widget.animationController,
          isPause: _isPause,
          toggleEvent: () {
            _isPause = !_isPause;
            return _isPause;
          },
          scaleNotifier: _scaleNotifier,
          setState: setState,
          forcePlay: () {
            setState(() {
              _isPause = false;
            });
          },
        ),
      ],
    );
  }
}
