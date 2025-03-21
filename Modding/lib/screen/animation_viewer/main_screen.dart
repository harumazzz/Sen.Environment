import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:screenshot/screenshot.dart';
import '../../bloc/selected_image_bloc/selected_image_bloc.dart';
import '../../bloc/selected_label_bloc/selected_label_bloc.dart';
import '../../bloc/selected_sprite_bloc/selected_sprite_bloc.dart';
import '../../cubit/initial_directory_cubit/initial_directory_cubit.dart';
import '../../extension/context.dart';
import '../../extension/platform.dart';
import '../../widget/collapsible_floating_button.dart';
import 'animation_screen.dart';
import 'label_info.dart';
import 'label_screen.dart';
import 'media_screen.dart';
import '../../service/file_helper.dart';
import 'visual_helper.dart';
import '../../i18n/app_localizations.dart';
import '../../service/ui_helper.dart';

class MainScreen extends StatefulWidget {
  const MainScreen({super.key});

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
  late TransformationController _transformationController;
  late AnimationController _animationController;
  late AnimationController _staticController;
  Matrix4? _matrix;
  String? _animationFile;
  String? _mediaDirectory;

  late VisualHelper _visualHelper;
  late ScreenshotController _screenshotController;

  late List<Widget> _screen;

  @override
  void initState() {
    _animationController = AnimationController(vsync: this);
    _staticController = AnimationController(vsync: this);
    _transformationController = TransformationController();
    _staticController.stop();
    _controller = TextEditingController();
    _visualHelper = VisualHelper(context: context);
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
    _transformationController.dispose();
    _animationController.dispose();
    _staticController.dispose();
    _controller.dispose();
    _visualHelper.dispose();
    super.dispose();
  }

  Future<void> _onUploadMedia() async {
    final los = AppLocalizations.of(context)!;
    final theme = Theme.of(context);
    await UIHelper.showFlutterDialog(
      context: context,
      child: UIHelper.buildDialog(
        title: Text(
          los.upload_media,
          style: theme.textTheme.titleMedium?.copyWith(
            fontWeight: FontWeight.w600,
          ),
        ),
        content: TextField(
          minLines: 1,
          maxLines: null,
          keyboardType: TextInputType.multiline,
          controller: _controller,
          decoration: InputDecoration(
            labelText: los.upload_directory,
            suffixIcon: IconButton(
              icon: Icon(
                Symbols.drive_folder_upload,
                color: theme.colorScheme.primary,
              ),
              onPressed: () async {
                void setDirectory(String directory) =>
                    BlocProvider.of<InitialDirectoryCubit>(
                      context,
                    ).setDirectoryOfDirectory(source: directory);
                final directory = await FileHelper.uploadDirectory(
                  initialDirectory:
                      BlocProvider.of<InitialDirectoryCubit>(
                        context,
                      ).state.initialDirectory,
                );
                if (directory != null) {
                  _controller.text = directory;
                  _mediaDirectory = directory;
                  setDirectory(directory);
                }
              },
            ),
            border: OutlineInputBorder(
              borderSide: BorderSide(color: theme.colorScheme.outline),
            ),
            focusedBorder: OutlineInputBorder(
              borderSide: BorderSide(
                color: theme.colorScheme.primary,
                width: 2.0,
              ),
            ),
          ),
        ),
        actions: [
          TextButton(
            onPressed: () async {
              pop() => Navigator.of(context).pop();
              await _visualHelper.loadImageSource(_controller.text);
              _visualHelper.hasMedia = true;
              pop();
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
    context.read<SelectedImageBloc>().add(
      SelectedImageAllocateEvent(size: _visualHelper.animation.image.length),
    );
    for (final sprite in _visualHelper.animation.sprite) {
      _sprite.add(sprite.name);
    }
    context.read<SelectedSpriteBloc>().add(
      SelectedSpriteAllocateEvent(size: _visualHelper.animation.sprite.length),
    );
    var labelName = 'main';
    _label.add(labelName);
    _visualHelper.labelInfo[labelName] = LabelInfo(
      startIndex: 0,
      endIndex: _visualHelper.animation.mainSprite.frame.length - 1,
    );
    for (
      var frameIndex = 0;
      frameIndex < _visualHelper.animation.mainSprite.frame.length;
      ++frameIndex
    ) {
      final frameLabelName =
          _visualHelper.animation.mainSprite.frame[frameIndex].label;
      if (frameLabelName != '' && frameLabelName != labelName) {
        labelName = frameLabelName;
        _visualHelper.labelInfo[labelName] = LabelInfo(
          startIndex: frameIndex,
          endIndex: frameIndex,
        );
        _label.add(labelName);
      }
      _visualHelper.labelInfo[labelName] = _visualHelper.labelInfo[labelName]!
          .copyWith(endIndex: _visualHelper.labelInfo[labelName]!.endIndex + 1);
    }
  }

  Future<void> _onErrorDialog(String message, StackTrace stack) async {
    final los = AppLocalizations.of(context)!;
    await UIHelper.showSimpleDialog(
      context: context,
      title: los.invalid_request,
      content: message,
    );
  }

  void _resetAnimation() {
    if (_animationController.duration != null) {
      _animationController.forward(from: 0.0);
    }
  }

  Future<void> _onUploadFile() async {
    void resetState() {
      context.read<SelectedLabelBloc>().add(const ResetLabelEvent());
      context.read<SelectedSpriteBloc>().add(const SelectedSpriteResetEvent());
      context.read<SelectedImageBloc>().add(const SelectedImageResetEvent());
    }

    _animationController.stop();
    void setFile(String file) =>
        context.read<InitialDirectoryCubit>().setDirectoryOfFile(source: file);
    final file = await FileHelper.uploadFile(
      initialDirectory:
          context.read<InitialDirectoryCubit>().state.initialDirectory,
    );
    if (file != null) {
      try {
        setFile(file);
        _animationFile = file;
        resetState();
        _resetAnimation();
        await _visualHelper.loadAnimation(file);
        _visualHelper.hasAnimation = true;
        if (_matrix == null) {
          final matrix = _transformationController.value;
          matrix[0] = 2;
          matrix[1] = 0;
          matrix[4] = 0;
          matrix[5] = 2;
          matrix[12] = -_visualHelper.animation.size.width * 4;
          matrix[13] = -_visualHelper.animation.size.height * 4;
          _matrix = matrix.clone();
          _transformationController.value = matrix;
        }
        await _onUploadMedia();
        _loadMedia();
        setState(_updateScreens);
      } catch (e, s) {
        await _onErrorDialog(e.toString(), s);
      }
    }
  }

  Future<void> _onDragFile(String file) async {
    await _visualHelper.loadAnimation(file);
    _visualHelper.hasAnimation = true;
    await _onUploadMedia();
    _resetAnimation();
    _loadMedia();
    setState(_updateScreens);
  }

  void _updateScreens() {
    _screen = <Widget>[
      AnimationScreen(
        key: ValueKey(_visualHelper.hasAnimation),
        controller: _screenshotController,
        visualHelper: _visualHelper,
        onUploadFile: _onUploadFile,
        onDragFile: _onDragFile,
        hasFile: false,
        matrix: _matrix,
        transformationController: _transformationController,
        animationController: _animationController,
        mediaDirectory: _mediaDirectory,
        sourceFile: _animationFile,
      ),
      LabelScreen(label: _label),
      MediaScreen(
        staticController: _staticController,
        visualHelper: _visualHelper,
        sprite: _sprite,
        image: _image,
        media: _media,
      ),
    ];
  }

  void _onItemTapped(int index) {
    setState(() {
      _selectedIndex = index;
    });
  }

  Widget? _navigationBar() {
    return UIHelper.ofMobile(
      builder: () {
        final los = AppLocalizations.of(context)!;
        return NavigationBar(
          destinations: <Widget>[
            NavigationDestination(
              icon: const Icon(Symbols.home),
              label: los.home,
            ),
            NavigationDestination(
              icon: const Icon(Symbols.label),
              label: los.label,
            ),
            NavigationDestination(
              icon: const Icon(Symbols.image),
              label: los.media,
            ),
          ],
          selectedIndex: _selectedIndex,
          onDestinationSelected: _onItemTapped,
        );
      },
    );
  }

  Widget _buildUI() {
    if (CurrentPlatform.isAndroid || CurrentPlatform.isIOS) {
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
                  color: Theme.of(context).colorScheme.surfaceContainerLow,
                  borderRadius: BorderRadius.circular(16),
                  boxShadow: [
                    BoxShadow(
                      color: Theme.of(
                        context,
                      ).shadowColor.withValues(alpha: 0.1),
                      blurRadius: 5,
                      offset: const Offset(0, 2),
                    ),
                  ],
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

  void _takeScreenshot() async {
    showScreenShot(Uint8List? value) async {
      if (value != null) {
        await UIHelper.showScreenshotDialog(context, value);
      }
    }

    final result = await _screenshotController.capture();
    await showScreenShot(result);
  }

  Widget _buildFloatingActionButton() {
    return CollapsibleFloatingActionButton(
      items: [
        CollapsibleFloatingActionButtonItem(
          icon: Symbols.file_upload,
          tooltip: context.los.upload_file,
          onPressed: _onUploadFile,
        ),
        CollapsibleFloatingActionButtonItem(
          icon: Symbols.screenshot,
          tooltip: context.los.take_screenshot,
          onPressed: _takeScreenshot,
        ),
      ],
    );
  }

  Future<void> _showContextMenu(TapDownDetails details) async {
    final overlay = Overlay.of(context).context.findRenderObject() as RenderBox;
    await showMenu(
      context: context,
      position: RelativeRect.fromRect(
        details.globalPosition & const Size(40, 40),
        Offset.zero & overlay.size,
      ),
      items: [
        PopupMenuItem(
          onTap: _takeScreenshot,
          child: Text(context.los.take_screenshot),
        ),
        PopupMenuItem(
          onTap: _onUploadFile,
          child: Text(context.los.upload_file),
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return GestureDetector(
      onSecondaryTapDown: _showContextMenu,
      child: Scaffold(
        appBar: UIHelper.ofMobile(
          builder: () {
            return AppBar(
              title: Text(
                los.animation_viewer,
                style: const TextStyle(fontWeight: FontWeight.bold),
              ),
              actions: [
                IconButton(
                  onPressed: _takeScreenshot,
                  icon: const Icon(Symbols.screenshot),
                ),
                IconButton(
                  onPressed: _onUploadFile,
                  icon: const Icon(Symbols.file_upload),
                ),
              ],
            );
          },
        ),
        body: _buildUI(),
        floatingActionButton: UIHelper.ofDesktop(
          builder: _buildFloatingActionButton,
        ),
        bottomNavigationBar: _navigationBar(),
      ),
    );
  }
}
