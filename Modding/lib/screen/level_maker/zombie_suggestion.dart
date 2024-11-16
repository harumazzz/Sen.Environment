import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/service/file_service.dart';
import 'package:path/path.dart' as p;
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class ZombieSelection extends StatefulWidget {
  const ZombieSelection({
    super.key,
    required this.zombies,
    required this.controller,
  });

  final List<String> zombies;

  final TextEditingController controller;

  @override
  State<ZombieSelection> createState() => _ZombieSelectionState();
}

class _ZombieSelection {
  String zombie;
  Image image;
  _ZombieSelection({
    required this.zombie,
    required this.image,
  });
}

class _ZombieSelectionState extends State<ZombieSelection> {
  late List<_ZombieSelection> _zombies;

  @override
  void initState() {
    _zombies = widget.zombies
        .map(
          (e) => _ZombieSelection(
            zombie: p.withoutExtension(p.basename(e)),
            image: Image(
              image: MemoryImage(
                FileService.readBuffer(source: e),
              ),
            ),
          ),
        )
        .toList();
    super.initState();
  }

  @override
  void dispose() {
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Autocomplete<_ZombieSelection>(
      optionsBuilder: (TextEditingValue textEditingValue) {
        if (textEditingValue.text.isEmpty) {
          return const Iterable<_ZombieSelection>.empty();
        }
        return _zombies.where(
          (_ZombieSelection e) {
            return e.zombie
                .toLowerCase()
                .contains(textEditingValue.text.toLowerCase());
          },
        );
      },
      onSelected: (_ZombieSelection e) {
        widget.controller.text = e.zombie;
      },
      fieldViewBuilder: (
        BuildContext context,
        TextEditingController textEditingController,
        FocusNode focusNode,
        VoidCallback onFieldSubmitted,
      ) {
        if (textEditingController.text.isEmpty) {
          textEditingController.text = widget.controller.text;
        }
        if (textEditingController.text != widget.controller.text) {
          widget.controller.text = textEditingController.text;
        }
        return Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            Expanded(
              child: TextField(
                controller: textEditingController,
                focusNode: focusNode,
              ),
            ),
            const SizedBox(width: 20),
            Tooltip(
              message: los.pick_a_zombie,
              child: IconButton(
                onPressed: () async {
                  await showDialog(
                    context: context,
                    builder: (context) => AlertDialog(
                      title: Text(los.pick_a_zombie),
                      content: ZombiePicker(
                        zombies: widget.zombies,
                        controller: textEditingController,
                        onChange: (String value, _) {
                          setState(() {
                            textEditingController.text = value;
                          });
                          Navigator.of(context).pop();
                        },
                      ),
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
                icon: const Icon(Symbols.add_2),
              ),
            ),
          ],
        );
      },
      optionsViewBuilder: (context, onSelected, options) {
        return Material(
          child: ListView.builder(
            padding: EdgeInsets.zero,
            itemCount: options.length,
            itemBuilder: (context, index) {
              final option = options.elementAt(index);
              return ListTile(
                leading: option.image,
                title: Text(option.zombie),
                onTap: () {
                  onSelected(option);
                },
              );
            },
          ),
        );
      },
      displayStringForOption: (option) => option.zombie,
    );
  }
}

class ZombiePicker extends StatefulWidget {
  const ZombiePicker({
    super.key,
    required this.zombies,
    required this.controller,
    required this.onChange,
  });
  final List<String> zombies;
  final TextEditingController controller;
  final void Function(String value, MemoryImage? image) onChange;

  @override
  State<ZombiePicker> createState() => _ZombiePickerState();
}

class _ZombiePickerState extends State<ZombiePicker> {
  late List<_ZombieSelection> _zombies;

  @override
  void initState() {
    _zombies = widget.zombies
        .map(
          (e) => _ZombieSelection(
            zombie: p.withoutExtension(p.basename(e)),
            image: Image(
              width: 50,
              height: 50,
              image: MemoryImage(
                FileService.readBuffer(source: e),
              ),
            ),
          ),
        )
        .toList();
    super.initState();
  }

  @override
  void dispose() {
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      child: Wrap(
        spacing: 8.0,
        runSpacing: 8.0,
        children: _zombies.map((option) {
          return GestureDetector(
            onTap: () => widget.onChange(
              option.zombie,
              option.image.image as MemoryImage,
            ),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                option.image,
              ],
            ),
          );
        }).toList(),
      ),
    );
  }
}
