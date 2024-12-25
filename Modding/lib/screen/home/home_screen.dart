import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/model/item.dart';
import 'package:sen/provider/level_provider.dart';
import 'package:sen/provider/map_provider.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/animation_viewer/main_screen.dart';
import 'package:sen/screen/javascript_category/js_pick.dart';
import 'package:sen/screen/level_maker/level_maker.dart';
import 'package:sen/screen/map_editor/map_editor.dart';
import 'package:sen/screen/shell/shell_screen.dart';
import 'package:page_transition/page_transition.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/service/file_service.dart';
import 'package:sen/widget/animated_floating.dart';

part 'grid_card.dart';
part 'list_card.dart';

class HomeScreen extends ConsumerStatefulWidget {
  const HomeScreen({super.key});

  @override
  ConsumerState<HomeScreen> createState() => _HomeScreenState();
}

class _HomeScreenState extends ConsumerState<HomeScreen> {
  late List<Item> items;

  @override
  void initState() {
    super.initState();
  }

  void _initWidget() {
    _initShellWidget();
    _initJSModule();
    _initAnimationViewer();
    _initLevelMaker();
    _initMapEditor();
  }

  void _initAnimationViewer() {
    items[2].onWidget = () {
      return const AnimationViewer();
    };
  }

  void _initShellWidget() {
    items[0].onWidget = () {
      return const ShellScreen(arguments: []);
    };
  }

  void _initJSModule() {
    final setting = ref.read(settingProvider);
    items[1].onWidget = () {
      return JsPick(
        holder: setting.toolChain,
      );
    };
  }

  void _onLoadLevelMakerConfiguration() async {
    await showModalBottomSheet<void>(
      context: context,
      builder: (BuildContext context) => const _LevelMakerConfiguration(),
    );
  }

  void _onLoadMapEditorConfiguration() async {
    await showModalBottomSheet<void>(
      context: context,
      builder: (BuildContext context) => const _MapEditorConfiguration(),
    );
  }

  void _initLevelMaker() {
    items[3].onWidget = () {
      return const LevelMaker();
    };
    items[3].onSetting = _onLoadLevelMakerConfiguration;
  }

  void _initMapEditor() {
    items[4].onWidget = () {
      return const MapEditor();
    };
    items[4].onSetting = _onLoadMapEditorConfiguration;
  }

  Widget _buildUI() {
    final los = AppLocalizations.of(context)!;
    if (Platform.isMacOS || Platform.isLinux || Platform.isWindows) {
      final screenWidth = MediaQuery.of(context).size.width;
      const itemWidth = 250.0;
      final crossAxisCount = (screenWidth / itemWidth).floor();

      return GridView.builder(
        gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: crossAxisCount,
          childAspectRatio: 1.0,
        ),
        itemCount: items.length,
        itemBuilder: (context, index) {
          final item = items[index];
          return GridCard(
            item: item,
            isValid: ref.watch(settingProvider).isValid,
            invalidMessage: los.toolchain_is_invalid,
            onSetting: item.onSetting,
          );
        },
      );
    } else {
      return ListView.builder(
        itemCount: items.length,
        itemBuilder: (context, index) {
          final item = items[index];
          return ListCard(
            item: item,
            isValid: ref.watch(settingProvider).isValid,
            invalidMessage: los.toolchain_is_invalid,
            onSetting: item.onSetting,
          );
        },
      );
    }
  }

  void _initItem() {
    final los = AppLocalizations.of(context)!;
    items = [
      Item(
        title: los.shell,
        description: los.shell_description,
        icon: Icon(
          Symbols.terminal_rounded,
          size: 50,
          color: Colors.blueGrey.shade600,
        ),
      ),
      Item(
        title: los.js_execute,
        description: los.js_execute_description,
        icon: Icon(
          Symbols.javascript_rounded,
          size: 50,
          color: Colors.yellow.shade600,
        ),
      ),
      Item(
        title: los.animation_viewer,
        description: los.animation_viewer_description,
        icon: Icon(
          Symbols.animated_images,
          size: 50,
          color: Colors.green.shade700,
        ),
      ),
      Item(
        title: los.level_maker,
        description: los.level_maker_description,
        icon: Icon(
          Symbols.build,
          size: 50,
          color: Colors.cyan.shade600,
        ),
      ),
      Item(
        title: los.map_editor,
        description: los.map_editor_description,
        icon: Icon(
          Symbols.map,
          size: 50,
          color: Colors.lightBlue.shade600,
        ),
      ),
    ];
  }

  @override
  Widget build(BuildContext context) {
    _initItem();
    _initWidget();
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: _buildUI(),
    );
  }
}

//map
class _MapEditorConfiguration extends ConsumerStatefulWidget {
  const _MapEditorConfiguration();

  @override
  ConsumerState<_MapEditorConfiguration> createState() =>
      __MapEditorConfigurationState();
}

class __MapEditorConfigurationState
    extends ConsumerState<_MapEditorConfiguration> {
  late TextEditingController _resourceLocationController;

  @override
  void initState() {
    super.initState();
    _resourceLocationController = TextEditingController(text: '');
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  @override
  void dispose() {
    _resourceLocationController.dispose();
    super.dispose();
  }

  Future<void> _onValueChange() async {
    final value = _resourceLocationController.text;
    await ref.watch(mapProvider.notifier).setResourceLocation(value);
  }

  void _onChangeSetting(String? value) async {
    if (value == null) return;
    _resourceLocationController.text = value;
    await _onValueChange();
  }

  void _onUploadDirectory() async {
    var result = await FileService.uploadDirectory();
    if (result != null) {
      _resourceLocationController.text = result;
      await _onValueChange();
    }
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    if (ref.watch(mapProvider).isLoading) {
      return const CircularProgressIndicator.adaptive();
    } else {
      final value = ref.watch(mapProvider).resourceLocation;
      if (value != null) {
        _resourceLocationController.text = value;
      }
    }
    return Padding(
      padding: const EdgeInsets.symmetric(
        horizontal: 12.0,
        vertical: 8.0,
      ),
      child: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.start,
          crossAxisAlignment: CrossAxisAlignment.start,
          children: <Widget>[
            const SizedBox(height: 12.0),
            Text(
              los.settings,
              style: Theme.of(context)
                  .textTheme
                  .titleLarge
                  ?.copyWith(fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 12.0),
            Row(
              children: [
                Text('${los.resource_location}: '),
                const SizedBox(width: 8.0),
                Expanded(
                  child: TextField(
                    controller: _resourceLocationController,
                    onChanged: _onChangeSetting,
                  ),
                ),
                const SizedBox(width: 4.0),
                IconButton(
                  onPressed: _onUploadDirectory,
                  icon: Tooltip(
                    message: los.upload_directory,
                    child: const Icon(Icons.folder),
                  ),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}

//level
class _LevelMakerConfiguration extends ConsumerStatefulWidget {
  const _LevelMakerConfiguration();

  @override
  ConsumerState<_LevelMakerConfiguration> createState() =>
      __LevelMakerConfigurationState();
}

class __LevelMakerConfigurationState
    extends ConsumerState<_LevelMakerConfiguration> {
  late TextEditingController _resourceLocationController;

  @override
  void initState() {
    super.initState();
    _resourceLocationController = TextEditingController(text: '');
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  @override
  void dispose() {
    _resourceLocationController.dispose();
    super.dispose();
  }

  Future<void> _onValueChange() async {
    final value = _resourceLocationController.text;
    await ref.watch(levelProvider.notifier).setResourceLocation(value);
  }

  void _onChangeSetting(String? value) async {
    if (value == null) return;
    _resourceLocationController.text = value;
    await _onValueChange();
  }

  void _onUploadDirectory() async {
    var result = await FileService.uploadDirectory();
    if (result != null) {
      _resourceLocationController.text = result;
      await _onValueChange();
    }
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    if (ref.watch(levelProvider).isLoading) {
      return const CircularProgressIndicator.adaptive();
    } else {
      final value = ref.watch(levelProvider).resourceLocation;
      if (value != null) {
        _resourceLocationController.text = value;
      }
    }
    return Padding(
      padding: const EdgeInsets.symmetric(
        horizontal: 12.0,
        vertical: 8.0,
      ),
      child: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.start,
          crossAxisAlignment: CrossAxisAlignment.start,
          children: <Widget>[
            const SizedBox(height: 12.0),
            Text(
              los.settings,
              style: Theme.of(context)
                  .textTheme
                  .titleLarge
                  ?.copyWith(fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 12.0),
            Row(
              children: [
                Text('${los.resource_location}: '),
                const SizedBox(width: 8.0),
                Expanded(
                  child: TextField(
                    controller: _resourceLocationController,
                    onChanged: _onChangeSetting,
                  ),
                ),
                const SizedBox(width: 4.0),
                IconButton(
                  onPressed: _onUploadDirectory,
                  icon: Tooltip(
                    message: los.upload_directory,
                    child: const Icon(Icons.folder),
                  ),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
