import 'dart:io';

import 'package:desktop_drop/desktop_drop.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/screen/animation_viewer/control_panel.dart';
import 'package:sen/screen/animation_viewer/label_screen.dart';
import 'package:sen/screen/animation_viewer/media_screen.dart';
import 'package:sen/screen/animation_viewer/provider/selected_image.dart';
import 'package:sen/screen/animation_viewer/provider/selected_label.dart';
import 'package:sen/screen/animation_viewer/provider/selected_sprite.dart';
import 'package:sen/screen/animation_viewer/visual_helper.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class AnimationScreen extends ConsumerStatefulWidget {
  const AnimationScreen({
    super.key,
    required this.hasFile,
    required this.onUploadFile,
    required this.onDragFile,
    required this.mediaScreen,
    required this.labelScreen,
    required this.animationController,
    required this.animationFile,
    required this.mediaDirectory,
  });

  final bool hasFile;

  final void Function() onUploadFile;

  final void Function(String value) onDragFile;

  final String? animationFile;

  final String? mediaDirectory;

  final MediaScreen mediaScreen;

  final LabelScreen labelScreen;

  final AnimationController animationController;

  @override
  ConsumerState<AnimationScreen> createState() => _AnimationScreenState();
}

class _AnimationScreenState extends ConsumerState<AnimationScreen> {
  late ValueNotifier<double> _xOffsetNotifier;
  late ValueNotifier<double> _yOffsetNotifier;
  bool _dragging = false;
  late ValueNotifier<double> _scaleNotifier;

  late Widget? _animationVisual;
  bool _isPause = false;

  @override
  void initState() {
    _scaleNotifier = ValueNotifier(1.0);
    _xOffsetNotifier = ValueNotifier(0.0);
    _yOffsetNotifier = ValueNotifier(0.0);
    super.initState();
    _animationVisual = null;
  }

  @override
  void dispose() {
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
          if (VisualHelper.hasAnimation) {
            VisualHelper.workingFrameRate = VisualHelper.animation.frameRate.toDouble();
          }
        }
      },
      child: data,
    );
  }

  void _showInfo() async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.info),
        content: SingleChildScrollView(
          scrollDirection: Axis.vertical,
          child: Column(
            mainAxisSize: MainAxisSize.min,
            mainAxisAlignment: MainAxisAlignment.start,
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(los.animation_file(widget.animationFile ?? los.unselected)),
              const SizedBox(height: 10.0),
              Text(los.media_directory(widget.mediaDirectory ?? los.unselected)),
              const SizedBox(height: 10.0),
              Row(
                crossAxisAlignment: CrossAxisAlignment.start,
                mainAxisAlignment: MainAxisAlignment.start,
                children: [
                  ElevatedButton(
                    child: Text(los.enable_all_image),
                    onPressed: () {
                      ref.read(selectedImageListProvider.notifier).enableAll();
                    },
                  ),
                  const SizedBox(width: 10.0),
                  ElevatedButton(
                    child: Text(los.disable_all_image),
                    onPressed: () {
                      ref.read(selectedImageListProvider.notifier).disableAll();
                    },
                  ),
                  const SizedBox(width: 10.0),
                  ElevatedButton(
                    child: Text(los.enable_all_sprite),
                    onPressed: () {
                      ref.read(selectedSpriteListNotifier.notifier).enableAll();
                    },
                  ),
                  const SizedBox(width: 10.0),
                  ElevatedButton(
                    child: Text(los.disable_all_sprite),
                    onPressed: () {
                      ref.read(selectedSpriteListNotifier.notifier).disableAll();
                    },
                  ),
                ],
              ),
            ],
          ),
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.of(context).pop(),
            child: Text(los.close),
          ),
        ],
      ),
    );
  }

  void _loadWorkingSprite(int index) {
    setState(() {
      final labelInfo = VisualHelper.labelInfo[ref.read<String>(selectedLabel)]!;
      final duration = ((labelInfo.endIndex - labelInfo.startIndex) / VisualHelper.workingFrameRate * 1000).toInt();
      widget.animationController.duration = Duration(milliseconds: duration);
      _animationVisual = VisualHelper.visualizeSprite(index, widget.animationController, ref);
      _animationVisual = SizedBox.fromSize(
        size: Size(VisualHelper.animation.size.width, VisualHelper.animation.size.height),
        child: UnconstrainedBox(
          child: SizedOverflowBox(
            alignment: AlignmentDirectional.topStart,
            size: Size(VisualHelper.animation.size.width, VisualHelper.animation.size.height),
            child: _animationVisual,
          ),
        ),
      );
      if (!_isPause) {
        widget.animationController.repeat();
      }
    });
  }

  Widget _buildMainContainer({
    required double scale,
    required double xOffset,
    required double yOffset,
  }) {
    final los = AppLocalizations.of(context)!;
    return Stack(
      fit: StackFit.passthrough,
      children: [
        ClipRRect(
          borderRadius: BorderRadius.circular(20),
          child: Transform(
            transform: VisualHelper.transformMatrixFromVariant([
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
        Positioned(
          top: 10,
          right: 10,
          child: Tooltip(
            message: los.info,
            child: IconButton(
              onPressed: _showInfo,
              icon: const Icon(Symbols.info, size: 24),
            ),
          ),
        ),
      ],
    );
  }

  Widget _buildAnimationVisual() {
    return ValueListenableBuilder<double>(
      valueListenable: _yOffsetNotifier,
      builder: (context, yOffset, child) {
        return ValueListenableBuilder<double>(
          valueListenable: _xOffsetNotifier,
          builder: (context, xOffset, child) => ValueListenableBuilder<double>(
            valueListenable: _scaleNotifier,
            builder: (context, scale, child) {
              return _buildMainContainer(scale: scale, xOffset: xOffset, yOffset: yOffset);
            },
          ),
        );
      },
    );
  }

  Widget _painterOrUpload() {
    if (VisualHelper.hasAnimation && VisualHelper.hasMedia) {
      _loadWorkingSprite(VisualHelper.animation.sprite.length);
      return _buildAnimationVisual();
    } else {
      return ClipRRect(
        clipBehavior: Clip.hardEdge,
        child: _dragAndDropWidget(_uploadFile()),
      );
    }
  }

  Widget _buildSliderRow({
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
                style: Theme.of(context).textTheme.titleSmall!.copyWith(fontWeight: FontWeight.bold),
              ),
              const SizedBox(width: 8.0),
              Expanded(
                child: Tooltip(
                  message: value.toStringAsFixed(2),
                  child: Slider(
                    value: value.clamp(min, max),
                    min: min,
                    max: max,
                    onChanged: (val) {
                      notifier.value = val;
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
    final los = AppLocalizations.of(context)!;
    return Column(
      children: [
        _buildContainer(),
        Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              _buildSliderRow(
                label: los.x,
                notifier: _xOffsetNotifier,
                min: -800,
                max: 800,
              ),
              const SizedBox(height: 4.0),
              const Divider(),
              const SizedBox(height: 4.0),
              _buildSliderRow(
                label: los.y,
                notifier: _yOffsetNotifier,
                min: -400,
                max: 400,
              ),
            ],
          ),
        ),
        ControlPanel(
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
            }),
      ],
    );
  }
}
