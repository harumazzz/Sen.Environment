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
          width: 600,
          height: 430,
          child: Padding(
            padding: const EdgeInsets.fromLTRB(10, 16, 8, 16),
            child: Row(
              children: [
                SizedBox(
                  width: 290,
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
                      )
                    ],
                  ),
                ),
                SizedBox(
                  width: 290,
                  child: Column(
                    children: [
                      ShortcutTile(
                        shortcutName: los.delete,
                        keyMapList: const ['Del'],
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
              ],
            ),
          ),
        ),
      ),
    );
  }
}

class ShortcutTile extends StatelessWidget {
  const ShortcutTile({super.key, required this.shortcutName, required this.keyMapList});

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
                    fontWeight: FontWeight.bold, color: Theme.of(context).colorScheme.onSecondaryFixedVariant),
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
          style: TextStyle(fontWeight: FontWeight.bold, color: Theme.of(context).colorScheme.onSecondaryFixedVariant),
        ),
      ),
    );
  }
}
