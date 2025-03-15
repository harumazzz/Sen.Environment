import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import '../../../../extension/context.dart';
import '../../../../service/ui_helper.dart';

class ShortCutMenuWidget extends StatelessWidget {
  const ShortCutMenuWidget({super.key});

  @override
  Widget build(BuildContext context) {
    final los = context.los;
    return AlertDialog(
      title: Text(los.shortcut_menu),
      content: SizedBox(
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
                  shortcutName: los.move_up,
                  keyMapList: const ['W'],
                ),
                ShortcutTile(
                  shortcutName: los.move_left,
                  keyMapList: const ['A'],
                ),
                ShortcutTile(
                  shortcutName: los.move_down,
                  keyMapList: const ['S'],
                ),
                ShortcutTile(
                  shortcutName: los.move_right,
                  keyMapList: const ['D'],
                ),
                ShortcutTile(
                  shortcutName: los.rotate_counter_clockwise,
                  keyMapList: const ['Z'],
                ),
                ShortcutTile(
                  shortcutName: los.rotate_clockwise,
                  keyMapList: const ['X'],
                ),
                ShortcutTile(
                  shortcutName: los.decrease_scale_x,
                  keyMapList: const ['J'],
                ),
                ShortcutTile(
                  shortcutName: los.increase_scale_x,
                  keyMapList: const ['K'],
                ),
                ShortcutTile(
                  shortcutName: los.decrease_scale_y,
                  keyMapList: const ['N'],
                ),
                ShortcutTile(
                  shortcutName: los.increase_scale_y,
                  keyMapList: const ['M'],
                ),
                ShortcutTile(shortcutName: los.flip, keyMapList: const ['F']),
                ShortcutTile(
                  shortcutName: los.cut,
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
                  shortcutName: los.section_select,
                  keyMapList: const ['Ctrl', '1'],
                ),
                ShortcutTile(
                  shortcutName: los.section_island_image,
                  keyMapList: const ['Ctrl', '2'],
                ),
                ShortcutTile(
                  shortcutName: los.section_island_animation,
                  keyMapList: const ['Ctrl', '3'],
                ),
                ShortcutTile(
                  shortcutName: los.section_event,
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
                ),
              ],
            ),
          ),
        ),
      ),
      actions: [...UIHelper.buildSimpleAction(context: context)],
    );
  }
}

class ShortcutTile extends StatelessWidget {
  const ShortcutTile({
    super.key,
    required this.shortcutName,
    required this.keyMapList,
  });

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
                color: Theme.of(context).colorScheme.onSurfaceVariant,
              ),
            ),
            const Spacer(),
            ...keyMapList.map((e) => KeyCap(keyName: e)),
          ],
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('shortcutName', shortcutName));
    properties.add(IterableProperty<String>('keyMapList', keyMapList));
  }
}

class KeyCap extends StatelessWidget {
  const KeyCap({super.key, required this.keyName});

  final String keyName;

  @override
  Widget build(BuildContext context) {
    return Card(
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(8)),
      color: Theme.of(context).colorScheme.secondaryContainer,
      child: Padding(
        padding: const EdgeInsets.fromLTRB(12, 4, 12, 4),
        child: Text(
          keyName,
          style: TextStyle(
            fontWeight: FontWeight.bold,
            color: Theme.of(context).colorScheme.onSurfaceVariant,
          ),
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('keyName', keyName));
  }
}
