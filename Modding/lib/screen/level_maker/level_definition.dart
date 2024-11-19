import 'package:flutter/material.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/model/wave.dart';
import 'package:sen/screen/level_maker/music_type.dart';

class LevelDefinition extends StatelessWidget {
  const LevelDefinition({
    super.key,
    required this.levelNameController,
    required this.levelDescriptionController,
    required this.levelStageController,
    required this.lawnMowerController,
    required this.musicType,
    required this.onChangeMusicType,
    required this.hasSunFalling,
    required this.onToggleSunFalling,
    required this.levelModule,
  });

  String _exchangeMusicType(BuildContext context, MusicType music) {
    final los = AppLocalizations.of(context)!;
    return switch (music) {
      MusicType.normal => los.level_music_normal,
      MusicType.ultimateBattle => los.level_music_ultimate,
      MusicType.minigame => los.level_music_minigame,
    };
  }

  final TextEditingController levelNameController;

  final TextEditingController levelDescriptionController;

  final TextEditingController levelStageController;

  final TextEditingController lawnMowerController;

  final LevelModule levelModule;

  final MusicType musicType;

  final void Function(MusicType? newValue) onChangeMusicType;

  final bool hasSunFalling;

  final void Function(bool? newValue) onToggleSunFalling;

  Widget _autoCompleter(
    List<String> options,
    TextEditingController controller,
  ) {
    return Autocomplete<String>(
      optionsBuilder: (TextEditingValue textEditingValue) {
        if (textEditingValue.text == '') {
          return const Iterable<String>.empty();
        }
        return options.where(
          (String option) {
            return option.contains(textEditingValue.text.toLowerCase());
          },
        );
      },
      onSelected: (String e) {
        controller.text = e;
      },
      fieldViewBuilder: (
        BuildContext context,
        TextEditingController textEditingController,
        FocusNode focusNode,
        VoidCallback onFieldSubmitted,
      ) {
        if (textEditingController.text.isEmpty) {
          textEditingController.text = controller.text;
        }
        if (textEditingController.text != controller.text) {
          controller.text = textEditingController.text;
        }
        return TextField(
          controller: textEditingController,
          focusNode: focusNode,
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Padding(
      padding: const EdgeInsets.all(16.0),
      child: Card(
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(16.0),
        ),
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Table(
            columnWidths: const {
              0: IntrinsicColumnWidth(),
              1: IntrinsicColumnWidth(),
            },
            children: [
              _buildTableRow(
                  context,
                  los.level_name,
                  TextField(
                    controller: levelNameController,
                  )),
              _buildTableRow(
                context,
                los.level_description,
                TextField(
                  controller: levelDescriptionController,
                ),
              ),
              _buildTableRow(
                context,
                los.level_stage,
                _autoCompleter(levelModule.stage!, levelStageController),
              ),
              _buildTableRow(
                context,
                los.lawn_mower,
                _autoCompleter(levelModule.mower!, lawnMowerController),
              ),
              _buildTableRow(
                  context,
                  los.level_music,
                  DropdownButton<MusicType>(
                    value: musicType,
                    isExpanded: true,
                    focusColor: Colors.transparent,
                    borderRadius: const BorderRadius.all(Radius.circular(10)),
                    underline: Container(),
                    items: MusicType.values
                        .map((e) => DropdownMenuItem<MusicType>(
                              value: e,
                              child: Text(_exchangeMusicType(context, e)),
                            ))
                        .toList(),
                    onChanged: onChangeMusicType,
                  )),
              TableRow(
                children: [
                  Padding(
                    padding: const EdgeInsets.symmetric(vertical: 16.0),
                    child: Text(
                      los.level_has_sun_falling,
                      style: Theme.of(context).textTheme.titleSmall,
                    ),
                  ),
                  const SizedBox(width: 15.0),
                  Padding(
                    padding: const EdgeInsets.symmetric(vertical: 8.0),
                    child: Align(
                      alignment: Alignment.centerLeft,
                      child: Checkbox(
                        value: hasSunFalling,
                        onChanged: onToggleSunFalling,
                        activeColor: Colors.teal,
                        checkColor: Colors.black,
                      ),
                    ),
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }

  TableRow _buildTableRow(BuildContext context, String label, Widget widget) {
    return TableRow(
      children: [
        Padding(
          padding: const EdgeInsets.symmetric(vertical: 16.0),
          child: Text(
            label,
            style: Theme.of(context).textTheme.titleSmall,
          ),
        ),
        const SizedBox(width: 15.0),
        Padding(
          padding: const EdgeInsets.symmetric(vertical: 8.0),
          child: widget,
        ),
      ],
    );
  }
}
