import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'package:sen/screen/map_editor/app/l10n/l10n.dart';
import 'package:sen/screen/map_editor/bloc/item/item_bloc.dart';
import 'package:sen/screen/map_editor/bloc/section/section_bloc.dart';
import 'package:sen/screen/map_editor/bloc/setting/setting_bloc.dart';
import 'package:sen/screen/map_editor/include/check_box_field.dart';
import 'package:sen/screen/map_editor/models/config.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class EditorSettingWidget extends StatelessWidget {
  const EditorSettingWidget({super.key});

  @override
  Widget build(BuildContext context) {
    final item = context
        .read<MapEditorConfigurationCubit>()
        .state
        .extensionItem[ExtensionType.setting]!;
    return SizedBox(
        width: 300,
        height: 520,
        child: Card(
          color: Theme.of(context).colorScheme.surface,
          child: Padding(
            padding:
                const EdgeInsets.only(top: 4, right: 16, left: 16, bottom: 16),
            child: Column(
              children: [
                SizedBox(
                  height: 40,
                  child: Row(
                    children: [
                      Container(
                          margin: const EdgeInsets.symmetric(horizontal: 10),
                          child: item.icon),
                      Text(
                        item.title,
                        style: const TextStyle(fontWeight: FontWeight.bold),
                      ),
                      const Spacer(),
                      IconButton(
                          onPressed: () {
                            context.read<SectionBloc>().add(
                                const ExtensionToggled(
                                    type: ExtensionType.setting,
                                    enabled: false));
                          },
                          icon: const Icon(Icons.close))
                    ],
                  ),
                ),
                Expanded(
                    child: Container(
                        decoration: BoxDecoration(
                          borderRadius: BorderRadius.circular(12),
                          color:
                              Theme.of(context).colorScheme.secondaryContainer,
                        ),
                        child: const EditorSettingState())),
              ],
            ),
          ),
        ));
  }
}

class EditorSettingState extends StatelessWidget {
  const EditorSettingState({super.key});

  String _backgroundString(BorderBackground background, AppLocalizations los) {
    switch (background) {
      //TODO: add locale.
      case BorderBackground.color:
        {
          return 'Color';
        }
      case BorderBackground.timeSpace:
        {
          return 'Time Space';
        }
      case BorderBackground.senLogo:
        {
          return 'Sen Logo';
        }
    }
  }

  @override
  Widget build(BuildContext context) {
    final los = context.los;
    return BlocBuilder<SettingBloc, SettingState>(builder: (context, state) {
      return Column(children: [
        CheckBoxField(
          label: 'Island Image Border', //TODO: add locale.
          value: state.islandImageBorder,
          margin:
              const EdgeInsets.only(left: 16, right: 16, bottom: 8, top: 16),
          underline: false,
          onChanged: (value) {
            context
                .read<SettingBloc>()
                .add(SetIslandImageBorder(enabled: value ?? true));
            context.read<ItemBloc>().add(const ItemStoreUpdated());
          },
        ),
        CheckBoxField(
          label: 'Island Animation Border', //TODO: add locale.
          value: state.islandAnimationBorder,
          margin: const EdgeInsets.only(left: 16, right: 16, bottom: 8, top: 8),
          underline: false,
          onChanged: (value) {
            context
                .read<SettingBloc>()
                .add(SetIslandAnimationBorder(enabled: value ?? true));
            context.read<ItemBloc>().add(const ItemStoreUpdated());
          },
        ),
        CheckBoxField(
          label: los.event_border,
          value: state.eventBorder,
          margin: const EdgeInsets.only(left: 16, right: 16, bottom: 8, top: 8),
          underline: false,
          onChanged: (value) {
            context
                .read<SettingBloc>()
                .add(SetEventBorder(enabled: value ?? true));
            context.read<ItemBloc>().add(const ItemStoreUpdated());
          },
        ),
        CheckBoxField(
          label: 'Hide Missing Art', //TODO: add locale.
          value: state.hideMissingArt,
          margin: const EdgeInsets.only(left: 16, right: 16, bottom: 8, top: 8),
          underline: false,
          onChanged: (value) {
            context
                .read<SettingBloc>()
                .add(SetHideMissingArt(enabled: value ?? true));
            context.read<ItemBloc>().add(const ItemStoreUpdated());
          },
        ),
        CheckBoxField(
          label: 'Hide Old Event', //TODO: add locale.
          value: state.hideOldEvent,
          margin: const EdgeInsets.only(left: 16, right: 16, bottom: 8, top: 8),
          underline: false,
          onChanged: (value) {
            context
                .read<SettingBloc>()
                .add(SetHideOldEvent(enabled: value ?? true));
            context.read<ItemBloc>().add(const ItemStoreUpdated());
          },
        ),
        CheckBoxField(
          label: 'Map Path', //TODO: add locale.
          value: state.mapPath,
          margin: const EdgeInsets.only(left: 16, right: 16, bottom: 8, top: 8),
          underline: false,
          onChanged: (value) {
            context.read<SettingBloc>().add(SetMapPath(enabled: value ?? true));
            context.read<ItemBloc>().add(const ItemStoreUpdated());
          },
        ),
        CheckBoxField(
          label: 'Map Completed', //TODO: add locale.
          value: state.mapCompleted,
          margin: const EdgeInsets.only(left: 16, right: 16, bottom: 8, top: 8),
          underline: false,
          onChanged: (value) {
            context
                .read<SettingBloc>()
                .add(SetMapCompleted(enabled: value ?? true));
            context.read<ItemBloc>().add(const ItemStoreUpdated());
          },
        ),
        CheckBoxField(
          label: los.map_grid, //TODO: add locale.
          value: state.mapGrid,
          margin: const EdgeInsets.only(left: 16, right: 16, bottom: 8, top: 8),
          underline: false,
          onChanged: (value) {
            context.read<SettingBloc>().add(SetMapGrid(enabled: value ?? true));
            context.read<ItemBloc>().add(const ItemStoreUpdated());
          },
        ),
        Padding(
            padding:
                const EdgeInsets.only(left: 16, right: 16, bottom: 8, top: 2),
            child: Row(
              children: [
                Text('Background', //TODO: add locale
                    style: TextStyle(
                        fontWeight: FontWeight.bold,
                        color: Theme.of(context)
                            .colorScheme
                            .onSecondaryFixedVariant)),
                const Spacer(),
                SizedBox(
                  width: 130,
                  child: DropdownButton<BorderBackground>(
                    value: state.boundBackground,
                    isExpanded: true,
                    focusColor: Colors.transparent,
                    underline: DecoratedBox(
                        decoration: BoxDecoration(
                            border: Border(
                                bottom: BorderSide(
                      color:
                          Theme.of(context).colorScheme.onSecondaryFixedVariant,
                      width: 0.8,
                    )))),
                    items: BorderBackground.values
                        .map((e) => DropdownMenuItem<BorderBackground>(
                              value: e,
                              child: Text(
                                _backgroundString(e, context.los),
                                maxLines: 1,
                                overflow: TextOverflow.ellipsis,
                                softWrap: true,
                              ),
                            ))
                        .toList(),
                    onChanged: (value) {
                      final background = value ?? BorderBackground.color;
                      context
                          .read<SettingBloc>()
                          .add(SetBoundBackground(background: background));
                    },
                  ),
                )
              ],
            )),
      ]);
    });
  }
}
