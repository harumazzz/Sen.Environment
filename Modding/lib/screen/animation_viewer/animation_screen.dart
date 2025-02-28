import 'dart:io';

import 'package:desktop_drop/desktop_drop.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:screenshot/screenshot.dart';
import 'package:sen/bloc/selected_image_bloc/selected_image_bloc.dart';
import 'package:sen/bloc/selected_label_bloc/selected_label_bloc.dart';
import 'package:sen/bloc/selected_sprite_bloc/selected_sprite_bloc.dart';
import 'package:sen/extension/context.dart';
import 'package:sen/extension/platform.dart';
import 'package:sen/screen/animation_viewer/control_panel.dart';
import 'package:sen/screen/animation_viewer/media_screen.dart';
import 'package:sen/screen/animation_viewer/visual_helper.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/service/file_helper.dart';
import 'package:path/path.dart' as p;

class AnimationScreen extends StatefulWidget {
  const AnimationScreen({
    super.key,
    required this.hasFile,
    required this.onUploadFile,
    required this.onDragFile,
    required this.mediaScreen,
    required this.animationController,
    required this.visualHelper,
    required this.selectedLabelBloc,
    required this.controller,
    required this.mediaDirectory,
    required this.sourceFile,
  });

  final bool hasFile;

  final String? mediaDirectory;

  final String? sourceFile;

  final Future<void> Function() onUploadFile;

  final void Function(String value) onDragFile;

  final MediaScreen mediaScreen;

  final AnimationController animationController;

  final VisualHelper visualHelper;

  final SelectedLabelBloc selectedLabelBloc;

  final ScreenshotController controller;

  @override
  State<AnimationScreen> createState() => _AnimationScreenState();
}

class _AnimationScreenState extends State<AnimationScreen> {
  bool _dragging = false;
  late ValueNotifier<double> _scaleNotifier;
  late ValueNotifier<double> _frameNotifier;
  late TransformationController _transformationController;

  late Widget? _animationVisual;
  late GlobalKey<PopupMenuButtonState<String>> _popupMenuKey;
  bool _isPause = false;

  Matrix4? _matrix;

  double _maxFrame = 0;

  @override
  void initState() {
    _scaleNotifier = ValueNotifier(1.0);
    _frameNotifier = ValueNotifier(0.0);
    _popupMenuKey = GlobalKey<PopupMenuButtonState<String>>();
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
        child: _dragging ? Text(los.drop_file_to_upload) : Text(los.upload_file_to_continue),
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
            widget.visualHelper.workingFrameRate = widget.visualHelper.animation.frameRate.toDouble();
          }
        }
      },
      child: data,
    );
  }

  void _loadWorkingSprite(int index) {
    final labelInfo = widget.visualHelper.labelInfo[widget.selectedLabelBloc.state.label]!;
    final duration =
        ((labelInfo.endIndex - labelInfo.startIndex) / widget.visualHelper.workingFrameRate * 1000).toInt();
    widget.animationController.duration = Duration(milliseconds: duration);
    _animationVisual = widget.visualHelper.visualizeSprite(index, widget.animationController);
    _animationVisual = SizedBox.fromSize(
      size: Size(widget.visualHelper.animation.size.width, widget.visualHelper.animation.size.height),
      child: UnconstrainedBox(
        child: SizedOverflowBox(
          alignment: AlignmentDirectional.topStart,
          size: Size(widget.visualHelper.animation.size.width, widget.visualHelper.animation.size.height),
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
    if (_matrix == null) {
      final matrix = _transformationController.value;
      matrix[0] = 2;
      matrix[1] = 0;
      matrix[4] = 0;
      matrix[5] = 2;
      matrix[12] = -widget.visualHelper.animation.size.width * 4;
      matrix[13] = -widget.visualHelper.animation.size.height * 4;
      _matrix = matrix.clone();
      _transformationController.value = matrix;
    }
  }

  Widget _buildPathField({
    required IconData icon,
    required String text,
  }) {
    return InkWell(
      onTap: CurrentPlatform.isAndroid
          ? null
          : () async {
              var destination = null as String?;
              if (icon == Symbols.folder) {
                destination = (text);
              } else {
                destination = (p.dirname(text));
              }
              await FileHelper.revealFile(destination);
            },
      child: Tooltip(
        message: context.los.open,
        child: Container(
          width: MediaQuery.of(context).size.width * 0.2,
          padding: const EdgeInsets.symmetric(horizontal: 12.0, vertical: 8.0),
          decoration: BoxDecoration(
            color: Theme.of(context).colorScheme.surfaceContainerHighest,
            borderRadius: BorderRadius.circular(8.0),
            border: Border.all(
              color: Theme.of(context).colorScheme.outlineVariant,
            ),
          ),
          child: Row(
            children: [
              Icon(
                icon,
                color: Theme.of(context).colorScheme.onSurfaceVariant,
                size: 20.0,
              ),
              const SizedBox(width: 8.0),
              Expanded(
                child: Text(
                  text,
                  overflow: TextOverflow.ellipsis,
                  maxLines: 5,
                  style: Theme.of(context).textTheme.bodyMedium,
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildMainAnimationScreen() {
    return SizedBox(
      width: double.infinity,
      child: Stack(
        children: [
          Screenshot(
            controller: widget.controller,
            child: ClipRRect(
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
                      child: _animationVisual,
                    ),
                  );
                },
              ),
            ),
          ),
          Positioned(
            top: 12.0,
            right: 8.0,
            child: Column(
              spacing: 5.0,
              children: [
                _buildPathField(
                  icon: Symbols.insert_drive_file,
                  text: widget.sourceFile ?? context.los.not_specified,
                ),
                _buildPathField(
                  icon: Symbols.folder,
                  text: widget.mediaDirectory ?? context.los.not_specified,
                ),
              ],
            ),
          ),
        ],
      ),
    );
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
        final textStyle = Theme.of(context).textTheme.titleSmall!.copyWith(fontWeight: FontWeight.bold);
        return Card(
          color: Theme.of(context).colorScheme.surfaceContainerHigh,
          elevation: 3,
          shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
          child: Padding(
            padding: const EdgeInsets.symmetric(
              horizontal: 16.0,
              vertical: 4.0,
            ),
            child: Row(
              children: [
                Text('$label: ', style: textStyle),
                const SizedBox(width: 8.0),
                Expanded(
                  child: Tooltip(
                    message: (value * _maxFrame).toStringAsFixed(0),
                    showDuration: Duration.zero,
                    child: Row(
                      children: [
                        Text(min.toStringAsFixed(0), style: textStyle),
                        Expanded(
                          child: Slider(
                            value: value,
                            min: min,
                            max: max,
                            onChanged: (val) => _onSliderChanged(val),
                          ),
                        ),
                        Text(_maxFrame.toStringAsFixed(0), style: textStyle),
                      ],
                    ),
                  ),
                ),
              ],
            ),
          ),
        );
      },
    );
  }

  void _onSliderChanged(double val) {
    if (_maxFrame > 0) {
      widget.animationController.animateTo(val, duration: Duration.zero);
      if (!_isPause) {
        widget.animationController.repeat();
      }
    }
  }

  Widget _buildContainer() {
    return Expanded(
      child: Container(
        width: double.infinity,
        margin: const EdgeInsets.symmetric(horizontal: 16.0),
        decoration: BoxDecoration(
          borderRadius: BorderRadius.circular(16.0),
          boxShadow: [
            BoxShadow(
              color: Colors.black.withValues(alpha: 0.08),
              blurRadius: 6,
              offset: const Offset(0, 4),
            ),
          ],
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
                label: los.frame,
                notifier: _frameNotifier,
                min: widget.animationController.lowerBound,
                max: widget.animationController.upperBound,
              )
            ],
          ),
        ),
        ControlPanel(
          popupMenuKey: _popupMenuKey,
          visualHelper: widget.visualHelper,
          animationController: widget.animationController,
          isPause: _isPause,
          toggleEvent: () {
            _isPause = !_isPause;
            return _isPause;
          },
          scaleNotifier: _scaleNotifier,
          setState: setState,
          resetFocus: () {
            setState(() {
              if (_matrix != null) {
                _transformationController.value = _matrix!.clone();
              }
            });
          },
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
