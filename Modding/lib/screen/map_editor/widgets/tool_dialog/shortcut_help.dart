import 'package:flutter/material.dart';
import 'package:sen/screen/map_editor/app/l10n/l10n.dart';

class ShortCutMenuWidget extends StatelessWidget {
  const ShortCutMenuWidget({super.key});

  @override
  Widget build(BuildContext context) {
    final los = context.los;
    return AlertDialog(
      title: Text(los.shortcut_menu),
      content: Card(
        shadowColor: Colors.transparent,
        child: SizedBox(
          width: 350,
          height: 450,
          child: SingleChildScrollView(
            child: Padding(
              padding: const EdgeInsets.fromLTRB(10, 8, 8, 8),
              child: Column(
                children: [
                  ShortcutTile(
                    shortcutName: los.open_world_map,
                    keyMapList: const ['Ctrl', 'O'],
                  ),
                  ShortcutTile(
                    shortcutName: los.save_world_map,
                    keyMapList: const ['Ctrl', 'S'],
                  ),
                  ShortcutTile(
                    shortcutName: los.rectangle_tool,
                    keyMapList: const ['Ctrl'],
                  ),
                  ShortcutTile(
                    shortcutName: los.erase_tool,
                    keyMapList: const ['E'],
                  ),
                  ShortcutTile(
                    shortcutName: los.pan_tool,
                    keyMapList: const ['Space'],
                  ),
                  ShortcutTile(
                    shortcutName: los.resize_tool,
                    keyMapList: const ['Ctrl', 'R'],
                  ),
                  ShortcutTile(
                    shortcutName: los.clear_tool,
                    keyMapList: const ['F8'],
                  ),
                  ShortcutTile(
                    shortcutName: los.open_config,
                    keyMapList: const ['F9'],
                  ),
                  ShortcutTile(
                    shortcutName: los.open_shortcut_menu,
                    keyMapList: const ['F2'],
                  ),
                  ShortcutTile(
                    shortcutName: los.delete,
                    keyMapList: const ['Del'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Move Up', //TODO: add locale
                    keyMapList: const ['W'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Move Left', //TODO: add locale
                    keyMapList: const ['A'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Move Down', //TODO: add locale
                    keyMapList: const ['S'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Move Right', //TODO: add locale
                    keyMapList: const ['D'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Rotate Counter Clockwise', //TODO: add locale
                    keyMapList: const ['Z'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Rotate Clockwise', //TODO: add locale
                    keyMapList: const ['X'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Decrease Scale X', //TODO: add locale
                    keyMapList: const ['J'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Increase Scale X', //TODO: add locale
                    keyMapList: const ['K'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Decrease Scale Y', //TODO: add locale
                    keyMapList: const ['N'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Increase Scale Y', //TODO: add locale
                    keyMapList: const ['M'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Flip', //TODO: add locale
                    keyMapList: const ['F'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Cut', //TODO: add locale
                    keyMapList: const ['Ctrl', 'X'],
                  ),
                  ShortcutTile(
                    shortcutName: los.copy,
                    keyMapList: const ['Ctrl', 'C'],
                  ),
                  ShortcutTile(
                    shortcutName: los.paste,
                    keyMapList: const ['Ctrl', 'V'],
                  ),
                  ShortcutTile(
                    shortcutName: los.undo,
                    keyMapList: const ['Ctrl', 'Z'],
                  ),
                  ShortcutTile(
                    shortcutName: los.redo,
                    keyMapList: const ['Ctrl', 'Y'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Section Select', //TODO: add locale
                    keyMapList: const ['Ctrl', '1'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Section Island image', //TODO: add locale
                    keyMapList: const ['Ctrl', '2'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Section Island animation', //TODO: add locale
                    keyMapList: const ['Ctrl', '3'],
                  ),
                  ShortcutTile(
                    shortcutName: 'Section Event', //TODO: add locale
                    keyMapList: const ['Ctrl', '4'],
                  ),
                  ShortcutTile(
                    shortcutName: los.open_layer,
                    keyMapList: const ['Ctrl', 'L'],
                  ),
                  ShortcutTile(
                    shortcutName: los.open_history,
                    keyMapList: const ['Ctrl', 'H'],
                  ),
                  ShortcutTile(
                    shortcutName: los.open_settings,
                    keyMapList: const ['Ctrl', 'I'],
                  ),
                  ShortcutTile(
                    shortcutName: los.open_palette,
                    keyMapList: const ['Ctrl', 'P'],
                  )
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }
}

class ShortcutTile extends StatelessWidget {
  const ShortcutTile(
      {super.key, required this.shortcutName, required this.keyMapList});

  final String shortcutName;

  final Iterable<String> keyMapList;

  @override
  Widget build(BuildContext context) {
    return Padding(
        padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 4),
        child: SizedBox(
          child: Row(
            children: [
              Text(
                shortcutName,
                style: TextStyle(
                    fontWeight: FontWeight.bold,
                    color: Theme.of(context).colorScheme.onSurfaceVariant),
              ),
              const Spacer(),
              ...keyMapList.map((e) => KeyCap(
                    keyName: e,
                  ))
            ],
          ),
        ));
  }
}

class KeyCap extends StatelessWidget {
  const KeyCap({super.key, required this.keyName});

  final String keyName;

  @override
  Widget build(BuildContext context) {
    return Card(
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(8),
      ),
      color: Theme.of(context).colorScheme.secondaryContainer,
      child: Padding(
        padding: const EdgeInsets.fromLTRB(12, 4, 12, 4),
        child: Text(
          keyName,
          style: TextStyle(
              fontWeight: FontWeight.bold,
              color: Theme.of(context).colorScheme.onSurfaceVariant),
        ),
      ),
    );
  }
}
