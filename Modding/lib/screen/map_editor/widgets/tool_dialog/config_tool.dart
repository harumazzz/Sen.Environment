import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'package:sen/extension/l10n.dart';
import 'package:sen/screen/map_editor/bloc/setting/setting_bloc.dart';
import 'package:sen/screen/map_editor/include/check_box_field.dart';

class ConfigSettingWidget extends StatefulWidget {
  const ConfigSettingWidget({super.key, required this.bloc});

  final SettingBloc bloc;

  @override
  State<StatefulWidget> createState() => _ConfigSettingState();
}

class _ConfigSettingState extends State<ConfigSettingWidget> {
  @override
  Widget build(BuildContext context) {
    final los = context.los;
    final bloc = widget.bloc;
    final isDesktopPlatform = context.read<MapEditorConfigurationCubit>().isDesktopPlatform;
    final topSpacing = isDesktopPlatform ? 16.0 : 4.0;
    final bottomSpacing = isDesktopPlatform ? 8.0 : 2.0;
    return AlertDialog(
      title: Text(los.map_editor_config),
      backgroundColor: Theme.of(context).colorScheme.onInverseSurface,
      content: Card(
        shadowColor: Colors.transparent,
        child: SizedBox(
          width: 300,
          child: SingleChildScrollView(
            scrollDirection: Axis.vertical,
            child: Column(
              children: [
                CheckBoxField(
                  label: context.los.play_single_animation_frame,
                  value: bloc.state.playSingleFrame,
                  margin: EdgeInsets.only(left: 16, right: 16, bottom: bottomSpacing, top: topSpacing),
                  underline: false,
                  onChanged: (value) {
                    bloc.add(SetPlaySingleFrame(playSingleFrame: value ?? false));
                    setState(() {});
                  },
                ),
                CheckBoxField(
                  label: los.mute_audio,
                  value: bloc.state.muteAudio,
                  margin: EdgeInsets.only(left: 16, right: 16, bottom: bottomSpacing, top: topSpacing),
                  underline: false,
                  onChanged: (value) {
                    bloc.add(SetMuteAudio(muteAudio: value ?? false));
                    setState(() {});
                  },
                ),
                CheckBoxField(
                  label: los.plant_costume,
                  value: bloc.state.plantCostume,
                  margin: EdgeInsets.only(left: 16, right: 16, bottom: bottomSpacing, top: topSpacing),
                  underline: false,
                  onChanged: (value) {
                    bloc.add(SetPlantCostume(enabled: value ?? false));
                    setState(() {});
                  },
                ),
                Padding(
                  padding: EdgeInsets.only(left: 16, right: 16, bottom: bottomSpacing, top: topSpacing),
                  child: Row(
                    children: [
                      Text(los.filter_quality,
                          style: TextStyle(
                              fontWeight: FontWeight.bold, color: Theme.of(context).colorScheme.onSurfaceVariant)),
                      const Spacer(),
                      SizedBox(
                        width: 100,
                        child: DropdownButton<FilterQuality>(
                          value: bloc.state.filterQuality,
                          isExpanded: true,
                          focusColor: Colors.transparent,
                          underline: DecoratedBox(
                              decoration: BoxDecoration(
                                  border: Border(
                                      bottom: BorderSide(
                            color: Theme.of(context).colorScheme.onSurfaceVariant,
                            width: 0.8,
                          )))),
                          items: FilterQuality.values
                              .map((e) => DropdownMenuItem<FilterQuality>(
                                    value: e,
                                    child: Text(
                                      e.name,
                                      maxLines: 1,
                                      overflow: TextOverflow.ellipsis,
                                      softWrap: true,
                                    ),
                                  ))
                              .toList(),
                          onChanged: (value) {
                            bloc.add(SetFilterQuality(filterQuality: value!));
                            setState(() {});
                          },
                        ),
                      ),
                    ],
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
      actions: [
        TextButton(
          child: Text(los.submit),
          onPressed: () {
            // returnValue = true;
            Navigator.of(context).pop(true);
          },
        ),
      ],
    );
  }
}
