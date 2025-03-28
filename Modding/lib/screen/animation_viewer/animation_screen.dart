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
import '../../service/ui_helper.dart';
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

  Future<void> _onDragDone(DropDoneDetails details) async {
    if (details.files.isNotEmpty) {
      final file = details.files.first;
      await widget.onDragFile(file.path);
      if (widget.visualHelper.hasAnimation) {
        widget.visualHelper.workingFrameRate =
            widget.visualHelper.animation.frameRate.toDouble();
      }
    }
  }

  void _onSliderChanged(double val) {
    if (_maxFrame > 0) {
      widget.animationController.animateTo(val, duration: Duration.zero);
      if (!_isPause) {
        widget.animationController.repeat();
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    context.watch<SelectedLabelBloc>();
    context.watch<SelectedImageBloc>();
    context.watch<SelectedSpriteBloc>();
    final los = AppLocalizations.of(context)!;
    return Column(
      children: [
        CustomAnimationContainer(
          child: () {
            if (widget.visualHelper.hasAnimation &&
                widget.visualHelper.hasMedia) {
              _loadWorkingSprite(widget.visualHelper.animation.sprite.length);
              return MainAnimationContainer(
                controller: widget.controller,
                width: widget.visualHelper.animation.size.width,
                height: widget.visualHelper.animation.size.height,
                transformationController: widget.transformationController,
                child: _animationVisual!,
                children: UIHelper.ofDesktop(
                  builder: () {
                    return [
                      CustomPathArea(
                        animationFile: widget.sourceFile,
                        mediaDirectory: widget.mediaDirectory,
                      ),
                      CustomBasicInformation(
                        height: widget.visualHelper.animation.size.height,
                        width: widget.visualHelper.animation.size.width,
                        scaleNotifier: _scaleNotifier,
                      ),
                      CustomControlInformation(
                        isPlaying: _isPlaying,
                        frameNotifier: _frameNotifier,
                        maxFrame: _maxFrame,
                        workingFrameRate: widget.visualHelper.workingFrameRate,
                      ),
                    ];
                  },
                ),
              );
            } else {
              return ClipRRect(
                clipBehavior: Clip.hardEdge,
                child: CustomDragAndDropRegion(
                  onDragDone: _onDragDone,
                  child: InkWell(
                    splashColor: Colors.blue.withAlpha(30),
                    onTap: widget.onUploadFile,
                    child: Center(
                      child: Text(context.los.upload_file_to_continue),
                    ),
                  ),
                ),
              );
            }
          }(),
        ),
        Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              CustomFrameSlider(
                label: los.frame,
                notifier: _frameNotifier,
                min: widget.animationController.lowerBound,
                max: widget.animationController.upperBound,
                maxFrame: _maxFrame,
                onChanged: _onSliderChanged,
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

class CustomAnimationContainer extends StatelessWidget {
  const CustomAnimationContainer({super.key, required this.child});

  final Widget child;

  @override
  Widget build(BuildContext context) {
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
        child: child,
      ),
    );
  }
}

class CustomPathField extends StatelessWidget {
  const CustomPathField({
    super.key,
    required this.icon,
    required this.text,
    required this.isDirectory,
  });

  final Widget icon;

  final String text;

  final bool isDirectory;

  Future<void> _onReveal({required String text}) async {
    var destination = null as String?;
    if (isDirectory) {
      destination = text;
    } else {
      destination = p.dirname(text);
    }
    await FileHelper.revealFile(destination);
  }

  @override
  Widget build(BuildContext context) {
    return InkWell(
      onTap:
          CurrentPlatform.isAndroid
              ? null
              : () async => await _onReveal(text: text),
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
              icon,
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

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('text', text));
    properties.add(DiagnosticsProperty<bool>('isDirectory', isDirectory));
  }
}

class CustomPathArea extends StatelessWidget {
  const CustomPathArea({
    super.key,
    required this.animationFile,
    required this.mediaDirectory,
  });

  final String? animationFile;

  final String? mediaDirectory;

  @override
  Widget build(BuildContext context) {
    return Positioned(
      top: 12.0,
      right: 8.0,
      child: Column(
        spacing: 5.0,
        children: [
          CustomPathField(
            icon: Icon(
              Symbols.insert_drive_file,
              color: Theme.of(context).colorScheme.onSurfaceVariant,
              size: 20.0,
            ),
            text: animationFile ?? context.los.not_specified,
            isDirectory: false,
          ),
          CustomPathField(
            icon: Icon(
              Symbols.folder,
              color: Theme.of(context).colorScheme.onSurfaceVariant,
              size: 20.0,
            ),
            text: mediaDirectory ?? context.los.not_specified,
            isDirectory: true,
          ),
        ],
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('animationFile', animationFile));
    properties.add(StringProperty('mediaDirectory', mediaDirectory));
  }
}

class InformationCard extends StatelessWidget {
  const InformationCard({
    super.key,
    required this.icon,
    required this.label,
    required this.value,
  });

  final IconData icon;

  final String label;

  final String value;

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.symmetric(horizontal: 12.0, vertical: 8.0),
        child: Row(
          spacing: 8.0,
          mainAxisSize: MainAxisSize.min,
          children: [
            Icon(icon),
            Text('$label:'),
            Expanded(
              child: Text(value, overflow: TextOverflow.ellipsis, maxLines: 4),
            ),
          ],
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<IconData>('icon', icon));
    properties.add(StringProperty('label', label));
    properties.add(StringProperty('value', value));
  }
}

class CustomBasicInformation extends StatelessWidget {
  const CustomBasicInformation({
    super.key,
    required this.width,
    required this.height,
    required this.scaleNotifier,
  });

  final double width;

  final double height;

  final ValueListenable<double> scaleNotifier;

  @override
  Widget build(BuildContext context) {
    return Positioned(
      bottom: 4.0,
      left: 4.0,
      child: SizedBox(
        width: 150,
        child: Column(
          spacing: 2.0,
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            InformationCard(
              icon: Symbols.image,
              label: context.los.width,
              value: width.toStringAsFixed(0),
            ),
            InformationCard(
              icon: Symbols.image,
              label: context.los.height,
              value: height.toStringAsFixed(0),
            ),
            ValueListenableBuilder<double>(
              valueListenable: scaleNotifier,
              builder: (context, value, child) {
                return InformationCard(
                  icon: Symbols.scale,
                  label: context.los.scale,
                  value: value.toStringAsFixed(1),
                );
              },
            ),
          ],
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DoubleProperty('width', width));
    properties.add(DoubleProperty('height', height));
    properties.add(
      DiagnosticsProperty<ValueListenable<double>>(
        'scaleNotifier',
        scaleNotifier,
      ),
    );
  }
}

class CustomControlInformation extends StatelessWidget {
  const CustomControlInformation({
    super.key,
    required this.workingFrameRate,
    required this.isPlaying,
    required this.frameNotifier,
    required this.maxFrame,
  });

  final double workingFrameRate;

  final bool isPlaying;

  final ValueListenable<double> frameNotifier;

  final double maxFrame;

  @override
  Widget build(BuildContext context) {
    return Positioned(
      bottom: 4.0,
      right: 4.0,
      child: SizedBox(
        width: 200,
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          spacing: 2.0,
          children: [
            InformationCard(
              icon: Symbols.frame_inspect,
              label: context.los.frame,
              value: workingFrameRate.toStringAsFixed(0),
            ),
            InformationCard(
              icon: Symbols.center_focus_strong,
              label: context.los.playing,
              value: isPlaying ? context.los.yes : context.los.no,
            ),
            ValueListenableBuilder<double>(
              valueListenable: frameNotifier,
              builder: (context, value, child) {
                return InformationCard(
                  icon: Symbols.playing_cards,
                  label: context.los.current,
                  value: (value * maxFrame).toStringAsFixed(0),
                );
              },
            ),
            InformationCard(
              icon: Symbols.label,
              label: context.los.label,
              value: context.read<SelectedLabelBloc>().state.label,
            ),
          ],
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DoubleProperty('workingFrameRate', workingFrameRate));
    properties.add(DiagnosticsProperty<bool>('isPlaying', isPlaying));
    properties.add(
      DiagnosticsProperty<ValueListenable<double>>(
        'frameNotifier',
        frameNotifier,
      ),
    );
    properties.add(DoubleProperty('maxFrame', maxFrame));
  }
}

class MainAnimationContainer extends StatelessWidget {
  const MainAnimationContainer({
    super.key,
    required this.controller,
    required this.transformationController,
    required this.width,
    required this.height,
    required this.child,
    this.children,
  });

  final ScreenshotController controller;

  final TransformationController transformationController;

  final double width;

  final double height;

  final Widget child;

  final List<Widget>? children;

  @override
  Widget build(BuildContext context) {
    return Stack(
      children: [
        Screenshot(
          controller: controller,
          child: ClipRRect(
            borderRadius: BorderRadius.circular(20),
            child: InteractiveViewer.builder(
              scaleFactor: 1000,
              maxScale: 10,
              minScale: 1.1,
              transformationController: transformationController,
              builder: (context, quad) {
                return Transform.scale(
                  scale: 0.5,
                  child: SizedBox(
                    width: width * 5,
                    height: height * 5,
                    child: child,
                  ),
                );
              },
            ),
          ),
        ),
        ...children ?? [],
      ],
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<ScreenshotController>('controller', controller),
    );
    properties.add(
      DiagnosticsProperty<TransformationController>(
        'transformationController',
        transformationController,
      ),
    );
    properties.add(DoubleProperty('width', width));
    properties.add(DoubleProperty('height', height));
  }
}

class CustomDragAndDropRegion extends StatefulWidget {
  const CustomDragAndDropRegion({
    super.key,
    required this.child,
    required this.onDragDone,
  });

  final Widget child;

  final Future<void> Function(DropDoneDetails details) onDragDone;

  @override
  State<CustomDragAndDropRegion> createState() =>
      _CustomDragAndDropRegionState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<Future<void> Function(DropDoneDetails details)>.has(
        'onDragDone',
        onDragDone,
      ),
    );
  }
}

class _CustomDragAndDropRegionState extends State<CustomDragAndDropRegion> {
  late bool _isDragging;

  @override
  void initState() {
    _isDragging = false;
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    if (CurrentPlatform.isAndroid || CurrentPlatform.isIOS) {
      return widget.child;
    }
    return DropTarget(
      onDragEntered: (details) {
        setState(() {
          _isDragging = true;
        });
      },
      onDragExited: (details) {
        setState(() {
          _isDragging = false;
        });
      },
      onDragDone: widget.onDragDone,
      child:
          _isDragging
              ? Center(child: Text(context.los.drop_file_to_upload))
              : widget.child,
    );
  }
}

class CustomFrameSlider extends StatelessWidget {
  const CustomFrameSlider({
    super.key,
    required this.label,
    required this.notifier,
    required this.min,
    required this.max,
    required this.maxFrame,
    required this.onChanged,
  });

  final String label;

  final ValueNotifier<double> notifier;

  final double min;

  final double max;

  final double maxFrame;

  final void Function(double value)? onChanged;

  @override
  Widget build(BuildContext context) {
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
                    message: (value * maxFrame).toStringAsFixed(0),
                    showDuration: Duration.zero,
                    child: Row(
                      children: [
                        Text(min.toStringAsFixed(0), style: textStyle),
                        Expanded(
                          child: Slider(
                            value: value,
                            min: min,
                            max: max,
                            onChanged: onChanged,
                          ),
                        ),
                        Text(maxFrame.toStringAsFixed(0), style: textStyle),
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

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('label', label));
    properties.add(
      DiagnosticsProperty<ValueNotifier<double>>('notifier', notifier),
    );
    properties.add(DoubleProperty('min', min));
    properties.add(DoubleProperty('max', max));
    properties.add(DoubleProperty('maxFrame', maxFrame));
    properties.add(
      ObjectFlagProperty<void Function(double value)?>.has(
        'onChanged',
        onChanged,
      ),
    );
  }
}
