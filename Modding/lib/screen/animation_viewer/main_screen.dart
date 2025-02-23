import 'dart:io';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:screenshot/screenshot.dart';
import 'package:sen/bloc/selected_image_bloc/selected_image_bloc.dart';
import 'package:sen/bloc/selected_label_bloc/selected_label_bloc.dart';
import 'package:sen/bloc/selected_sprite_bloc/selected_sprite_bloc.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/screen/animation_viewer/animation_screen.dart';
import 'package:sen/screen/animation_viewer/control_button.dart';
import 'package:sen/screen/animation_viewer/label_info.dart';
import 'package:sen/screen/animation_viewer/label_screen.dart';
import 'package:sen/screen/animation_viewer/media_screen.dart';
import 'package:sen/service/file_helper.dart';
import 'package:sen/screen/animation_viewer/visual_helper.dart';
import 'package:sen/i18n/app_localizations.dart';

class MainScreen extends StatefulWidget {
  const MainScreen({
    super.key,
    required this.initialDirectoryCubit,
    required this.selectedImageBloc,
    required this.selectedSpriteBloc,
    required this.selectedLabelBloc,
  });

  final InitialDirectoryCubit initialDirectoryCubit;

  final SelectedImageBloc selectedImageBloc;

  final SelectedSpriteBloc selectedSpriteBloc;
  final SelectedLabelBloc selectedLabelBloc;

  @override
  State<MainScreen> createState() => _MainScreenState();
}

class _MainScreenState extends State<MainScreen> with TickerProviderStateMixin {
  int _selectedIndex = 0;

  late List<String> _sprite;
  late List<String> _image;
  late List<String> _media;
  late List<String> _label;
  late TextEditingController _controller;
  late AnimationController _animationController;
  late AnimationController _staticController;
  String? _animationFile;
  String? _mediaDirectory;

  late VisualHelper _visualHelper;
  late ScreenshotController _screenshotController;

  @override
  void initState() {
    _animationController = AnimationController(vsync: this);
    _staticController = AnimationController(vsync: this);
    _staticController.stop();
    _controller = TextEditingController();
    _visualHelper = VisualHelper(
      context: context,
    );
    _screenshotController = ScreenshotController();
    super.initState();
    _sprite = [];
    _image = [];
    _media = [];
    _label = [];
    _updateScreens();
  }

  @override
  void dispose() {
    _animationController.dispose();
    _staticController.dispose();
    _controller.dispose();
    _visualHelper.dispose();
    super.dispose();
  }

  Future<void> _onUploadMedia() async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.upload_media),
        content: Row(
          children: [
            const Icon(Symbols.abc),
            Expanded(
              child: TextField(
                controller: _controller,
              ),
            ),
            IconButton(
              onPressed: () async {
                final directory = await FileHelper.uploadDirectory(
                  initialDirectory: widget.initialDirectoryCubit.state.initialDirectory,
                );
                if (directory != null) {
                  _controller.text = directory;
                  _mediaDirectory = directory;
                  widget.initialDirectoryCubit.setDirectoryOfDirectory(source: directory);
                }
              },
              icon: const Icon(Symbols.drive_folder_upload),
            ),
          ],
        ),
        actions: [
          TextButton(
            onPressed: () {
              _visualHelper.loadImageSource(_controller.text);
              _visualHelper.hasMedia = true;
              Navigator.of(context).pop();
            },
            child: Text(los.okay),
          ),
        ],
      ),
    );
  }

  void _cleanUp() {
    _label.clear();
    _media.clear();
    _image.clear();
    _sprite.clear();
  }

  void _loadMedia() {
    _cleanUp();
    for (final image in _visualHelper.animation.image) {
      _image.add(image.path);
      _media.add('${image.path}.png');
    }
    widget.selectedImageBloc.add(
      SelectedImageAllocateEvent(
        size: _visualHelper.animation.image.length,
      ),
    );
    for (final sprite in _visualHelper.animation.sprite) {
      _sprite.add(sprite.name);
    }
    widget.selectedSpriteBloc.add(SelectedSpriteAllocateEvent(
      size: _visualHelper.animation.sprite.length,
    ));
    var labelName = 'main';
    _label.add(labelName);
    _visualHelper.labelInfo[labelName] = LabelInfo(
      startIndex: 0,
      endIndex: _visualHelper.animation.mainSprite.frame.length - 1,
    );
    for (var frameIndex = 0; frameIndex < _visualHelper.animation.mainSprite.frame.length; ++frameIndex) {
      final frameLabelName = _visualHelper.animation.mainSprite.frame[frameIndex].label;
      if (frameLabelName != '' && frameLabelName != labelName) {
        labelName = frameLabelName;
        _visualHelper.labelInfo[labelName] = LabelInfo(startIndex: frameIndex, endIndex: frameIndex);
        _label.add(labelName);
      }
      _visualHelper.labelInfo[labelName] = _visualHelper.labelInfo[labelName]!.copyWith(
        endIndex: _visualHelper.labelInfo[labelName]!.endIndex + 1,
      );
    }
  }

  void _onErrorDialog(String message, StackTrace stack) async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.invalid_request),
        content: Text(message),
        actions: [
          TextButton(
            onPressed: () async {
              Navigator.of(context).pop();
              await showDialog(
                context: context,
                builder: (context) => AlertDialog(
                  title: Text(message),
                  content: Text(stack.toString()),
                  actions: [
                    TextButton(
                      onPressed: () {
                        Navigator.of(context).pop();
                      },
                      child: Text(los.okay),
                    ),
                  ],
                ),
              );
            },
            child: Text(los.detail),
          ),
          TextButton(
            onPressed: () {
              Navigator.of(context).pop();
            },
            child: Text(los.okay),
          ),
        ],
      ),
    );
  }

  void _resetAnimation() {
    if (_animationController.duration != null) {
      _animationController.forward(from: 0.0);
    }
  }

  void _onUploadFile() async {
    void resetImageEvent() => widget.selectedImageBloc.add(const SelectedImageResetEvent());
    void resetSpriteEvent() => widget.selectedSpriteBloc.add(const SelectedSpriteResetEvent());
    void resetLabelEvent() => widget.selectedLabelBloc.add(const ResetLabelEvent());
    final file = await FileHelper.uploadFile(
      initialDirectory: widget.initialDirectoryCubit.state.initialDirectory,
    );
    if (file != null) {
      try {
        widget.initialDirectoryCubit.setDirectoryOfFile(source: file);
        _animationFile = file;
        resetLabelEvent();
        resetSpriteEvent();
        resetImageEvent();
        _resetAnimation();
        await _visualHelper.loadAnimation(file);
        _visualHelper.hasAnimation = true;
        await _onUploadMedia();
        _loadMedia();
        setState(() {
          _updateScreens();
        });
      } catch (e, s) {
        _onErrorDialog(e.toString(), s);
      }
    }
  }

  void _onDragFile(
    String file,
  ) async {
    await _visualHelper.loadAnimation(file);
    _visualHelper.hasAnimation = true;
    await _onUploadMedia();
    _resetAnimation();
    _loadMedia();
    setState(() {
      _updateScreens();
    });
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
            spacing: 10.0,
            children: [
              Text(los.animation_file(_animationFile ?? los.unselected)),
              Text(los.media_directory(_mediaDirectory ?? los.unselected)),
              Row(
                spacing: 10.0,
                crossAxisAlignment: CrossAxisAlignment.start,
                mainAxisAlignment: MainAxisAlignment.start,
                children: [
                  Text('${los.image}: '),
                  ControlButton(
                    tooltip: los.enable_all_image,
                    icon: Symbols.select_all,
                    onPressed: () {
                      widget.selectedImageBloc.add(const SelectedImageEnableAllEvent());
                    },
                  ),
                  ControlButton(
                    tooltip: los.disable_all_image,
                    icon: Symbols.deselect,
                    onPressed: () {
                      widget.selectedImageBloc.add(const SelectedImageDisableAllEvent());
                    },
                  )
                ],
              ),
              Row(
                crossAxisAlignment: CrossAxisAlignment.start,
                mainAxisAlignment: MainAxisAlignment.start,
                spacing: 10.0,
                children: [
                  Text('${los.sprite}: '),
                  ControlButton(
                    tooltip: los.enable_all_sprite,
                    icon: Symbols.select_all,
                    onPressed: () {
                      widget.selectedSpriteBloc.add(const SelectedSpriteEnableAllEvent());
                    },
                  ),
                  ControlButton(
                    tooltip: los.disable_all_sprite,
                    icon: Symbols.deselect,
                    onPressed: () {
                      widget.selectedSpriteBloc.add(const SelectedSpriteDisableAllEvent());
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

  void _updateScreens() {
    final mediaScreen = MediaScreen(
      staticController: _staticController,
      visualHelper: _visualHelper,
      sprite: _sprite,
      image: _image,
      media: _media,
    );
    final labelScreen = LabelScreen(label: _label);
    _screen = <Widget>[
      AnimationScreen(
        key: ValueKey(_visualHelper.hasAnimation),
        controller: _screenshotController,
        visualHelper: _visualHelper,
        selectedLabelBloc: widget.selectedLabelBloc,
        onUploadFile: _onUploadFile,
        onDragFile: _onDragFile,
        hasFile: false,
        mediaScreen: mediaScreen,
        labelScreen: labelScreen,
        animationController: _animationController,
      ),
      labelScreen,
      mediaScreen,
    ];
  }

  late List<Widget> _screen;

  void _onItemTapped(int index) {
    setState(() {
      _selectedIndex = index;
    });
  }

  Widget? _navigationBar() {
    final los = AppLocalizations.of(context)!;
    if (Platform.isMacOS || Platform.isWindows || Platform.isLinux) {
      return null;
    }
    return BottomNavigationBar(
      items: <BottomNavigationBarItem>[
        BottomNavigationBarItem(
          icon: const Icon(Symbols.home),
          label: los.home,
        ),
        BottomNavigationBarItem(
          icon: const Icon(Symbols.label),
          label: los.label,
        ),
        BottomNavigationBarItem(
          icon: const Icon(Symbols.image),
          label: los.media,
        ),
      ],
      currentIndex: _selectedIndex,
      onTap: _onItemTapped,
    );
  }

  Widget _buildUI() {
    if (Platform.isAndroid || Platform.isIOS) {
      return _screen[_selectedIndex];
    } else {
      return Padding(
        padding: const EdgeInsets.all(8.0),
        child: Row(
          children: [
            Flexible(
              flex: 4,
              child: Container(
                margin: const EdgeInsets.only(left: 8.0, bottom: 8.0),
                decoration: BoxDecoration(
                  borderRadius: BorderRadius.circular(20),
                  border: Border.all(
                    color: Theme.of(context).brightness == Brightness.dark
                        ? Colors.white.withValues(alpha: 0.3)
                        : Colors.black.withValues(alpha: 0.3),
                    width: 2,
                  ),
                ),
                child: Padding(
                  padding: const EdgeInsets.all(8.0),
                  child: _screen[1],
                ),
              ),
            ),
            Flexible(flex: 12, child: _screen[0]),
            Flexible(
              flex: 4,
              child: Container(
                margin: const EdgeInsets.only(left: 12.0, bottom: 12.0),
                child: _screen[2],
              ),
            ),
          ],
        ),
      );
    }
  }

  Future<void> _showDialog(
    Uint8List image,
  ) async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) {
        return AlertDialog(
          title: Text(los.screenshot_taken),
          content: Image.memory(
            image,
          ),
          actions: [
            TextButton(
              child: Text(los.save),
              onPressed: () async {
                void closeDialog() => Navigator.of(context).pop();
                var file = await FileHelper.saveFile(
                  initialDirectory: BlocProvider.of<InitialDirectoryCubit>(context).state.initialDirectory,
                  suggestedName: 'screenshot.png',
                );
                if (file != null) {
                  if (RegExp(r'\.png$', caseSensitive: false).hasMatch(file)) {
                    file += '.png';
                  }
                  FileHelper.writeBuffer(source: file, data: image);
                  closeDialog();
                }
              },
            ),
            TextButton(
              child: Text(los.okay),
              onPressed: () {
                Navigator.of(context).pop();
              },
            ),
          ],
        );
      },
    );
  }

  void _takeScreenshot() async {
    final imageBytes = await _screenshotController.capture();

    if (imageBytes != null) {
      await _showDialog(imageBytes);
    }
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Scaffold(
      appBar: AppBar(
        forceMaterialTransparency: Platform.isWindows || Platform.isLinux || Platform.isMacOS,
        title: Text(los.animation_viewer),
        actions: [
          IconButton(
            onPressed: _takeScreenshot,
            icon: const Icon(Symbols.screenshot),
          ),
          IconButton(
            onPressed: _onUploadFile,
            icon: const Icon(Symbols.file_upload),
          ),
          IconButton(
            onPressed: _showInfo,
            icon: const Icon(Symbols.info),
          ),
        ],
      ),
      body: _buildUI(),
      bottomNavigationBar: _navigationBar(),
    );
  }
}
