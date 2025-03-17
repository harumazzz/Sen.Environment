import 'package:desktop_drop/desktop_drop.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:screenshot/screenshot.dart';
import '../../bloc/selected_image_bloc/selected_image_bloc.dart';
import '../../bloc/selected_label_bloc/selected_label_bloc.dart';
import '../../bloc/selected_sprite_bloc/selected_sprite_bloc.dart';
import '../../extension/context.dart';
import '../../extension/platform.dart';
import 'control_panel.dart';
import 'visual_helper.dart';
import '../../i18n/app_localizations.dart';
import '../../service/file_helper.dart';
import 'package:path/path.dart' as p;

class AnimationScreen extends StatefulWidget {
  const AnimationScreen({
    super.key,
    required this.hasFile,
    required this.onUploadFile,
    required this.onDragFile,
    required this.animationController,
    required this.visualHelper,
    required this.controller,
    required this.matrix,
    required this.transformationController,
    required this.mediaDirectory,
    required this.sourceFile,
  });

  final bool hasFile;

  final String? mediaDirectory;

  final String? sourceFile;

  final Future<void> Function() onUploadFile;

  final Future<void> Function(String value) onDragFile;

  final Matrix4? matrix;

  final AnimationController animationController;

  final VisualHelper visualHelper;

  final ScreenshotController controller;

  final TransformationController transformationController;

  @override
  State<AnimationScreen> createState() => _AnimationScreenState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<bool>('hasFile', hasFile));
    properties.add(StringProperty('mediaDirectory', mediaDirectory));
    properties.add(StringProperty('sourceFile', sourceFile));
    properties.add(
      ObjectFlagProperty<Future<void> Function()>.has(
        'onUploadFile',
        onUploadFile,
      ),
    );
    properties.add(
      ObjectFlagProperty<void Function(String value)>.has(
        'onDragFile',
        onDragFile,
      ),
    );
    properties.add(TransformProperty('matrix', matrix));
    properties.add(
      DiagnosticsProperty<AnimationController>(
        'animationController',
        animationController,
      ),
    );
    properties.add(
      DiagnosticsProperty<VisualHelper>('visualHelper', visualHelper),
    );
    properties.add(
      DiagnosticsProperty<ScreenshotController>('controller', controller),
    );
    properties.add(
      DiagnosticsProperty<TransformationController>(
        'transformationController',
        transformationController,
      ),
    );
  }
}

class _AnimationScreenState extends State<AnimationScreen> {
  bool _dragging = false;
  late ValueNotifier<double> _scaleNotifier;
  late ValueNotifier<double> _frameNotifier;

  late Widget? _animationVisual;
  late GlobalKey<PopupMenuButtonState<String>> _popupMenuKey;
  bool _isPause = false;

  double _maxFrame = 0;

  bool get _isPlaying => !_isPause;

  @override
  void initState() {
    _scaleNotifier = ValueNotifier(1.0);
    _frameNotifier = ValueNotifier(0.0);
    _popupMenuKey = GlobalKey<PopupMenuButtonState<String>>();
    widget.transformationController.addListener(() {
      _scaleNotifier.value =
          widget.transformationController.value.getMaxScaleOnAxis() - 1.0;
    });
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
        child:
            _dragging
                ? Text(los.drop_file_to_upload)
                : Text(los.upload_file_to_continue),
      ),
    );
  }

  Widget _dragAndDropWidget(Widget data) {
    if (CurrentPlatform.isAndroid || CurrentPlatform.isIOS) {
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
      onDragDone: (details) async {
        if (details.files.isNotEmpty) {
          final file = details.files.first;
          await widget.onDragFile(file.path);
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
        widget.visualHelper.labelInfo[BlocProvider.of<SelectedLabelBloc>(
          context,
        ).state.label]!;
    final duration =
        ((labelInfo.endIndex - labelInfo.startIndex) /
                widget.visualHelper.workingFrameRate *
                1000)
            .toInt();
    widget.animationController.duration = Duration(milliseconds: duration);
    _animationVisual = widget.visualHelper.visualizeSprite(
      index,
      widget.animationController,
    );
    _animationVisual = SizedBox.fromSize(
      size: Size(
        widget.visualHelper.animation.size.width,
        widget.visualHelper.animation.size.height,
      ),
      child: UnconstrainedBox(
        child: SizedOverflowBox(
          alignment: AlignmentDirectional.topStart,
          size: Size(
            widget.visualHelper.animation.size.width,
            widget.visualHelper.animation.size.height,
          ),
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
  }

  Future<void> _onReveal({required IconData icon, required String text}) async {
    var destination = null as String?;
    if (icon == Symbols.folder) {
      destination = text;
    } else {
      destination = p.dirname(text);
    }
    await FileHelper.revealFile(destination);
  }

  Widget _buildPathField({required IconData icon, required String text}) {
    return InkWell(
      onTap:
          CurrentPlatform.isAndroid
              ? null
              : () async => await _onReveal(icon: icon, text: text),
      child: Tooltip(
        message: context.los.open,
        child: Container(
          width: MediaQuery.sizeOf(context).width * 0.18,
          padding: const EdgeInsets.symmetric(horizontal: 8.0, vertical: 6.0),
          decoration: BoxDecoration(
            color: Theme.of(context).colorScheme.surfaceContainerHighest,
            borderRadius: BorderRadius.circular(12.0),
            border: Border.all(
              color: Theme.of(context).colorScheme.outlineVariant,
            ),
          ),
          child: Row(
            spacing: 8.0,
            children: [
              Icon(
                icon,
                color: Theme.of(context).colorScheme.onSurfaceVariant,
                size: 20.0,
              ),
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
    final mainScreen = Screenshot(
      controller: widget.controller,
      child: ClipRRect(
        borderRadius: BorderRadius.circular(20),
        child: InteractiveViewer.builder(
          scaleFactor: 1000,
          maxScale: 10,
          minScale: 1.1,
          transformationController: widget.transformationController,
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
    );
    return Stack(
      children: [
        mainScreen,
        if (CurrentPlatform.isDesktop) _buildRegularPath(),
        if (CurrentPlatform.isDesktop) _buildBasicInformation(),
        if (CurrentPlatform.isDesktop) _buildControlInformation(),
      ],
    );
  }

  Widget _buildControlInformation() {
    return Positioned(
      bottom: 4.0,
      right: 4.0,
      child: SizedBox(
        width: 200,
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          spacing: 2.0,
          children: [
            _informationOf(
              icon: Symbols.frame_inspect,
              key: context.los.frame,
              value: widget.visualHelper.workingFrameRate.toStringAsFixed(0),
            ),
            _informationOf(
              icon: Symbols.center_focus_strong,
              key: context.los.playing,
              value: _isPlaying ? context.los.yes : context.los.no,
            ),
            ValueListenableBuilder<double>(
              valueListenable: _frameNotifier,
              builder: (context, value, child) {
                return _informationOf(
                  icon: Symbols.playing_cards,
                  key: context.los.current,
                  value: (value * _maxFrame).toStringAsFixed(0),
                );
              },
            ),
            _informationOf(
              icon: Symbols.label,
              key: context.los.label,
              value: context.read<SelectedLabelBloc>().state.label,
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildRegularPath() {
    return Positioned(
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
    );
  }

  Widget _informationOf({
    required IconData icon,
    required String key,
    required String value,
  }) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.symmetric(horizontal: 12.0, vertical: 8.0),
        child: Row(
          spacing: 8.0,
          mainAxisSize: MainAxisSize.min,
          children: [
            Icon(icon),
            Text('$key:'),
            Text(value, overflow: TextOverflow.ellipsis, maxLines: 4),
          ],
        ),
      ),
    );
  }

  Widget _buildBasicInformation() {
    return Positioned(
      bottom: 4.0,
      left: 4.0,
      child: SizedBox(
        width: 150,
        child: Column(
          spacing: 2.0,
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            _informationOf(
              icon: Symbols.image,
              key: context.los.width,
              value: widget.visualHelper.animation.size.width.toStringAsFixed(
                0,
              ),
            ),
            _informationOf(
              icon: Symbols.image,
              key: context.los.height,
              value: widget.visualHelper.animation.size.height.toStringAsFixed(
                0,
              ),
            ),
            ValueListenableBuilder<double>(
              valueListenable: _scaleNotifier,
              builder: (context, value, child) {
                return _informationOf(
                  icon: Symbols.scale,
                  key: context.los.scale,
                  value: value.toStringAsFixed(1),
                );
              },
            ),
          ],
        ),
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
        final textStyle = Theme.of(
          context,
        ).textTheme.titleSmall!.copyWith(fontWeight: FontWeight.bold);
        return Card(
          color: Theme.of(context).colorScheme.surfaceContainerHigh,
          elevation: 4.0,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(12),
          ),
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
                            onChanged: _onSliderChanged,
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
            color:
                Theme.of(context).brightness == Brightness.dark
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
              ),
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
            setState(() {});
            return _isPause;
          },
          setState: setState,
          resetFocus: () {
            setState(() {
              if (widget.matrix != null) {
                widget.transformationController.value = widget.matrix!.clone();
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
