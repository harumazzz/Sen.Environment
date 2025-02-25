import 'dart:io';

import 'package:flutter/material.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/model/item.dart';
import 'package:sen/screen/animation_viewer/animation_viewer.dart';
import 'package:sen/screen/home/configuration/javascript_category_configuration.dart';
import 'package:sen/screen/home/configuration/level_maker_configuration.dart';
import 'package:sen/screen/home/configuration/map_editor_configuration.dart';
import 'package:sen/screen/home/configuration/shell_configuration.dart';
import 'package:sen/screen/home/option_list.dart';
import 'package:sen/screen/javascript_category/javascript_category.dart';
import 'package:sen/screen/level_maker/level_maker.dart';
import 'package:sen/screen/map_editor/map_editor.dart';
import 'package:sen/screen/shell/shell_screen.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/widget/animated_floating.dart';

part 'grid_card.dart';
part 'list_card.dart';

class HomeScreen extends StatelessWidget {
  const HomeScreen({super.key});

  void _showJsSettings(BuildContext context) async {
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

  void _onLoadLevelMakerConfiguration(BuildContext context) async {
    await showModalBottomSheet<void>(
      context: context,
      builder: (BuildContext context) => const LevelMakerConfiguration(),
    );
  }

  void _onLoadMapEditorConfiguration(BuildContext context) async {
    await showModalBottomSheet<void>(
      context: context,
      builder: (BuildContext context) => const MapEditorConfiguration(),
    );
  }

  void _onLoadShellConfiguration(
    BuildContext context,
  ) async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text(los.shell_configuration),
          content: const ShellConfiguration(),
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

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    final items = [
      Item(
        title: los.shell,
        description: los.shell_description,
        icon: Icon(
          Symbols.terminal_rounded,
          size: 50,
          color: Colors.blueGrey.shade600,
        ),
        isEnabled: true,
        onWidget: () => const ShellScreen(),
        onSetting: () => _onLoadShellConfiguration(context),
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
        onWidget: () => const JavaScriptCategory(),
        onSetting: () => _showJsSettings(context),
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
        onWidget: () => const AnimationViewer(),
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
        onWidget: () => const LevelMaker(),
        onSetting: () => _onLoadLevelMakerConfiguration(context),
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
        onWidget: () => const MapEditor(),
        onSetting: () => _onLoadMapEditorConfiguration(context),
      ),
    ];

    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: OptionList(
        items: items,
      ),
    );
  }
}
