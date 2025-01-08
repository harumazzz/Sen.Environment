import 'dart:io';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/screen/animation_viewer/animation_screen.dart';
import 'package:sen/screen/animation_viewer/control_button.dart';
import 'package:sen/screen/animation_viewer/label_screen.dart';
import 'package:sen/screen/animation_viewer/media_screen.dart';
import 'package:sen/screen/animation_viewer/provider/selected_image.dart';
import 'package:sen/screen/animation_viewer/provider/selected_label.dart';
import 'package:sen/screen/animation_viewer/provider/selected_sprite.dart';
import 'package:sen/service/file_helper.dart';
import 'package:sen/screen/animation_viewer/visual_helper.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class AnimationViewer extends ConsumerStatefulWidget {
  const AnimationViewer({super.key});

  @override
  ConsumerState<AnimationViewer> createState() => _AnimationViewerState();
}

class _AnimationViewerState extends ConsumerState<AnimationViewer> with SingleTickerProviderStateMixin {
  int _selectedIndex = 0;

  late List<String> _sprite;
  late List<String> _image;
  late List<String> _media;
  late List<String> _label;
  late TextEditingController _controller;
  late AnimationController _animationController;
  String? _animationFile;
  String? _mediaDirectory;

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
                void setWorkingDirectory(String source) {
                  BlocProvider.of<InitialDirectoryCubit>(context).setDirectoryOfDirectory(source: source);
                }

                final directory = await FileHelper.uploadDirectory(
                  initialDirectory: BlocProvider.of<InitialDirectoryCubit>(context).state.initialDirectory,
                );
                if (directory != null) {
                  _controller.text = directory;
                  _mediaDirectory = directory;
                  setWorkingDirectory(directory);
                }
              },
              icon: const Icon(Symbols.drive_folder_upload),
            ),
          ],
        ),
        actions: [
          TextButton(
            onPressed: () {
              VisualHelper.loadImageSource(_controller.text);
              VisualHelper.hasMedia = true;
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
    for (final image in VisualHelper.animation.image) {
      _image.add(image.path);
      _media.add('${image.path}.png');
    }
    ref.read(selectedImageListProvider.notifier).allocate(VisualHelper.animation.image.length);
    for (final sprite in VisualHelper.animation.sprite) {
      _sprite.add(sprite.name);
    }
    ref.read(selectedSpriteListNotifier.notifier).allocate(VisualHelper.animation.sprite.length);
    var labelName = 'main';
    _label.add('main');
    VisualHelper.labelInfo[labelName] =
        LabelInfo(startIndex: 0, endIndex: VisualHelper.animation.mainSprite.frame.length - 1);
    for (var frameIndex = 0; frameIndex < VisualHelper.animation.mainSprite.frame.length; ++frameIndex) {
      final frameLabelName = VisualHelper.animation.mainSprite.frame[frameIndex].label;
      if (frameLabelName != '' && frameLabelName != labelName) {
        labelName = frameLabelName;
        VisualHelper.labelInfo[labelName] = LabelInfo(startIndex: frameIndex, endIndex: frameIndex);
        _label.add(labelName);
      }
      ++VisualHelper.labelInfo[labelName]!.endIndex;
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

  void _setWorkingDirectory(String source) {
    BlocProvider.of<InitialDirectoryCubit>(context).setDirectoryOfFile(source: source);
  }

  void _onUploadFile() async {
    final file = await FileHelper.uploadFile(
      initialDirectory: BlocProvider.of<InitialDirectoryCubit>(context).state.initialDirectory,
    );
    if (file != null) {
      try {
        _setWorkingDirectory(file);
        _animationFile = file;
        ref.read(selectedLabel.notifier).resetLabel();
        ref.read(selectedImageListProvider.notifier).reset();
        ref.read(selectedSpriteListNotifier.notifier).reset();
        _resetAnimation();
        VisualHelper.dispose();
        await VisualHelper.loadAnimation(file);
        VisualHelper.hasAnimation = true;
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
    await VisualHelper.loadAnimation(file);
    VisualHelper.hasAnimation = true;
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
            children: [
              Text(los.animation_file(_animationFile ?? los.unselected)),
              const SizedBox(height: 10.0),
              Text(los.media_directory(_mediaDirectory ?? los.unselected)),
              const SizedBox(height: 10.0),
              Row(
                crossAxisAlignment: CrossAxisAlignment.start,
                mainAxisAlignment: MainAxisAlignment.start,
                children: [
                  Text('${los.image}: '),
                  const SizedBox(width: 10.0),
                  ControlButton(
                    tooltip: los.enable_all_image,
                    icon: Symbols.select_all,
                    onPressed: () {
                      ref.read(selectedImageListProvider.notifier).enableAll();
                    },
                  ),
                  const SizedBox(width: 10.0),
                  ControlButton(
                    tooltip: los.disable_all_image,
                    icon: Symbols.deselect,
                    onPressed: () {
                      ref.read(selectedImageListProvider.notifier).disableAll();
                    },
                  )
                ],
              ),
              const SizedBox(height: 10.0),
              Row(
                crossAxisAlignment: CrossAxisAlignment.start,
                mainAxisAlignment: MainAxisAlignment.start,
                children: [
                  Text('${los.sprite}: '),
                  const SizedBox(width: 10.0),
                  ControlButton(
                    tooltip: los.enable_all_sprite,
                    icon: Symbols.select_all,
                    onPressed: () {
                      ref.read(selectedSpriteListNotifier.notifier).enableAll();
                    },
                  ),
                  const SizedBox(width: 10.0),
                  ControlButton(
                    tooltip: los.disable_all_sprite,
                    icon: Symbols.deselect,
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

  void _updateScreens() {
    final mediaScreen = MediaScreen(
      sprite: _sprite,
      image: _image,
      media: _media,
    );
    final labelScreen = LabelScreen(label: _label);
    _screen = <Widget>[
      AnimationScreen(
        key: ValueKey(VisualHelper.hasAnimation),
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

  @override
  void initState() {
    _animationController = AnimationController(vsync: this);
    _controller = TextEditingController();
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
    _controller.dispose();
    VisualHelper.dispose();
    super.dispose();
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

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Scaffold(
      appBar: AppBar(
        forceMaterialTransparency: Platform.isWindows || Platform.isLinux || Platform.isMacOS,
        title: Text(los.animation_viewer),
        actions: [
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
