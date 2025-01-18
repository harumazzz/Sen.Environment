import 'package:flutter/material.dart';
import 'package:sen/screen/map_editor/app/l10n/l10n.dart';

class ShortCutMenuWidget extends StatelessWidget {
  const ShortCutMenuWidget({super.key});

  @override
  Widget build(BuildContext context) {
    final los = context.los; //TODO: add locale
    return const AlertDialog(
        title: Text('Shortcut Menu'),
        content: Card(
          shadowColor: Colors.transparent,
          child: SizedBox(
              width: 600,
              height: 430,
              child: Padding(
                  padding: EdgeInsets.fromLTRB(10, 16, 8, 16),
                  child: Row(
                    children: [
                      SizedBox(
                        width: 290,
                        child: Column(
                          children: [
                            ShortcutTile(
                              shortcutName: 'Open WorldMap',
                              keyMapList: ['Ctrl', 'O'],
                            ),
                            ShortcutTile(
                              shortcutName: 'Save WorldMap',
                              keyMapList: ['Ctrl', 'S'],
                            ),
                            ShortcutTile(
                              shortcutName: 'Rectangle Tool',
                              keyMapList: ['Ctrl'],
                            ),
                            ShortcutTile(
                              shortcutName: 'Erase Tool',
                              keyMapList: ['E'],
                            ),
                            ShortcutTile(
                              shortcutName: 'Pan Tool',
                              keyMapList: ['Space'],
                            ),
                            ShortcutTile(
                              shortcutName: 'Resize Tool',
                              keyMapList: ['Ctrl', 'R'],
                            ),
                            ShortcutTile(
                              shortcutName: 'Clear Tool',
                              keyMapList: ['F8'],
                            ),
                            ShortcutTile(
                              shortcutName: 'Open Config',
                              keyMapList: ['F9'],
                            ),
                            ShortcutTile(
                              shortcutName: 'Open Shortcut Menu',
                              keyMapList: ['F2'],
                            )
                          ],
                        ),
                      ),
                      SizedBox(
                          width: 290,
                          child: Column(
                            children: [
                              ShortcutTile(
                                shortcutName: 'Delete',
                                keyMapList: ['Del'],
                              ),
                              ShortcutTile(
                                shortcutName: 'Copy',
                                keyMapList: ['Ctrl', 'C'],
                              ),
                              ShortcutTile(
                                shortcutName: 'Paste',
                                keyMapList: ['Ctrl', 'V'],
                              ),
                              ShortcutTile(
                                shortcutName: 'Undo',
                                keyMapList: ['Ctrl', 'Z'],
                              ),
                              ShortcutTile(
                                shortcutName: 'Redo',
                                keyMapList: ['Ctrl', 'Y'],
                              ),
                              ShortcutTile(
                                shortcutName: 'Open Layer',
                                keyMapList: ['Ctrl', 'L'],
                              ),
                              ShortcutTile(
                                shortcutName: 'Open History',
                                keyMapList: ['Ctrl', 'H'],
                              ),
                              ShortcutTile(
                                shortcutName: 'Open Setting',
                                keyMapList: ['Ctrl', 'I'],
                              ),
                              ShortcutTile(
                                shortcutName: 'Open Palette',
                                keyMapList: ['Ctrl', 'P'],
                              )
                            ],
                          ))
                    ],
                  ))),
        ));
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
                    color:
                        Theme.of(context).colorScheme.onSecondaryFixedVariant),
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
              color: Theme.of(context).colorScheme.onSecondaryFixedVariant),
        ),
      ),
    );
  }
}
