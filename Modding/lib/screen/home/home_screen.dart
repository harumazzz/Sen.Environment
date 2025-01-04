import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/model/item.dart';
import 'package:sen/screen/animation_viewer/main_screen.dart';
import 'package:sen/screen/home/configuration/javascript_category_configuration.dart';
import 'package:sen/screen/home/configuration/level_maker_configuration.dart';
import 'package:sen/screen/home/configuration/map_editor_configuration.dart';
import 'package:sen/screen/home/option_list.dart';
import 'package:sen/screen/javascript_category/javascript_category.dart';
import 'package:sen/screen/level_maker/level_maker.dart';
import 'package:sen/screen/map_editor/map_editor.dart';
import 'package:sen/screen/shell/shell_screen.dart';
import 'package:page_transition/page_transition.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
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
    items[1].onWidget = () {
      return const JavaScriptCategory();
    };
    items[1].onSetting = _showJsSettings;
  }

  void _showJsSettings() async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text(los.js_settings),
          content: const JavaScriptCategoryConfiguration(),
          actions: <Widget>[
            TextButton(
              onPressed: () {
                Navigator.of(context).pop();
              },
              child: Text(los.okay),
            ),
          ],
        );
      },
    );
  }

  void _onLoadLevelMakerConfiguration() async {
    await showModalBottomSheet<void>(
      context: context,
      builder: (BuildContext context) => const LevelMakerConfiguration(),
    );
  }

  void _onLoadMapEditorConfiguration() async {
    await showModalBottomSheet<void>(
      context: context,
      builder: (BuildContext context) => const MapEditorConfiguration(),
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
        isEnabled: true,
      ),
      Item(
        title: los.js_execute,
        description: los.js_execute_description,
        icon: Icon(
          Symbols.javascript_rounded,
          size: 50,
          color: Colors.yellow.shade600,
        ),
        isEnabled: true,
      ),
      Item(
        title: los.animation_viewer,
        description: los.animation_viewer_description,
        icon: Icon(
          Symbols.animated_images,
          size: 50,
          color: Colors.green.shade700,
        ),
        isEnabled: true,
      ),
      Item(
        title: los.level_maker,
        description: los.level_maker_description,
        icon: Icon(
          Symbols.build,
          size: 50,
          color: Colors.cyan.shade600,
        ),
        isEnabled: true,
      ),
      Item(
        title: los.map_editor,
        description: los.map_editor_description,
        icon: Icon(
          Symbols.map,
          size: 50,
          color: Colors.lightBlue.shade600,
        ),
        isEnabled: !Platform.isAndroid && !Platform.isIOS,
      ),
    ];
  }

  @override
  Widget build(BuildContext context) {
    _initItem();
    _initWidget();
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: OptionList(
        items: items,
      ),
    );
  }
}
