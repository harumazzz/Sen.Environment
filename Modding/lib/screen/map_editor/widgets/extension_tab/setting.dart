import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'package:sen/extension/context.dart';
import 'package:sen/extension/platform.dart';
import 'package:sen/screen/map_editor/bloc/item/item_bloc.dart';
import 'package:sen/screen/map_editor/bloc/section/section_bloc.dart';
import 'package:sen/screen/map_editor/bloc/setting/setting_bloc.dart';
import 'package:sen/screen/map_editor/include/check_box_field.dart';
import 'package:sen/screen/map_editor/models/config.dart';
import 'package:sen/i18n/app_localizations.dart';

class EditorSettingWidget extends StatelessWidget {
  const EditorSettingWidget({super.key});

  @override
  Widget build(BuildContext context) {
    final item =
        context
            .read<MapEditorConfigurationCubit>()
            .state
            .extensionItem[ExtensionType.setting]!;
    return SizedBox(
      width: 300,
      height: 400,
      child: Card(
        color: Theme.of(context).colorScheme.surface,
        child: Padding(
          padding: const EdgeInsets.only(
            top: 4,
            right: 16,
            left: 16,
            bottom: 16,
          ),
          child: Column(
            children: [
              SizedBox(
                height: 40,
                child: Row(
                  children: [
                    Container(
                      margin: const EdgeInsets.symmetric(horizontal: 10),
                      child: Icon(item.icon),
                    ),
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
                            enabled: false,
                          ),
                        );
                      },
                      icon: const Icon(Icons.close),
                    ),
                  ],
                ),
              ),
              Expanded(
                child: Container(
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.circular(12),
                    color: Theme.of(context).colorScheme.secondaryContainer,
                  ),
                  child: const EditorSettingState(),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

class EditorSettingState extends StatelessWidget {
  const EditorSettingState({super.key});

  String _backgroundString(BorderBackground background, AppLocalizations los) {
    switch (background) {
      case BorderBackground.color:
        {
          return los.color;
        }
      case BorderBackground.timeSpace:
        {
          return los.time_space;
        }
      case BorderBackground.senLogo:
        {
          return los.sui_logo;
        }
    }
  }

  @override
  Widget build(BuildContext context) {
    final los = context.los;
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    final spacing = isDesktopPlatform ? 8.0 : 2.0;
    return BlocBuilder<SettingBloc, SettingState>(
      builder: (context, state) {
        return SingleChildScrollView(
          child: Column(
            children: [
              if (!isDesktopPlatform)
                CheckBoxField(
                  label: context.los.screenshot_taken,
                  value: state.showTopScreenShortcut,
                  margin: EdgeInsets.only(
                    left: 16,
                    right: 16,
                    bottom: spacing,
                    top: 8,
                  ),
                  underline: false,
                  onChanged: (value) {
                    context.read<SettingBloc>().add(
                      SetShowScreenShortcut(enabled: value ?? true),
                    );
                  },
                ),
              CheckBoxField(
                label: los.island_image_border,
                value: state.islandImageBorder,
                margin: EdgeInsets.only(
                  left: 16,
                  right: 16,
                  bottom: spacing,
                  top: spacing,
                ),
                underline: false,
                onChanged: (value) {
                  context.read<SettingBloc>().add(
                    SetIslandImageBorder(enabled: value ?? true),
                  );
                  context.read<ItemBloc>().add(const ItemStoreUpdated());
                },
              ),
              CheckBoxField(
                label: los.animated_island_border,
                value: state.islandAnimationBorder,
                margin: EdgeInsets.only(
                  left: 16,
                  right: 16,
                  bottom: spacing,
                  top: spacing,
                ),
                underline: false,
                onChanged: (value) {
                  context.read<SettingBloc>().add(
                    SetIslandAnimationBorder(enabled: value ?? true),
                  );
                  context.read<ItemBloc>().add(const ItemStoreUpdated());
                },
              ),
              CheckBoxField(
                label: los.event_border,
                value: state.eventBorder,
                margin: EdgeInsets.only(
                  left: 16,
                  right: 16,
                  bottom: spacing,
                  top: spacing,
                ),
                underline: false,
                onChanged: (value) {
                  context.read<SettingBloc>().add(
                    SetEventBorder(enabled: value ?? true),
                  );
                  context.read<ItemBloc>().add(const ItemStoreUpdated());
                },
              ),
              CheckBoxField(
                label: los.hide_missing_art,
                value: state.hideMissingArt,
                margin: EdgeInsets.only(
                  left: 16,
                  right: 16,
                  bottom: spacing,
                  top: spacing,
                ),
                underline: false,
                onChanged: (value) {
                  context.read<SettingBloc>().add(
                    SetHideMissingArt(enabled: value ?? true),
                  );
                  context.read<ItemBloc>().add(const ItemStoreUpdated());
                },
              ),
              CheckBoxField(
                label: los.hide_old_events,
                value: state.hideOldEvent,
                margin: EdgeInsets.only(
                  left: 16,
                  right: 16,
                  bottom: spacing,
                  top: spacing,
                ),
                underline: false,
                onChanged: (value) {
                  context.read<SettingBloc>().add(
                    SetHideOldEvent(enabled: value ?? true),
                  );
                  context.read<ItemBloc>().add(const ItemStoreUpdated());
                },
              ),
              CheckBoxField(
                label: los.use_map_path,
                value: state.mapPath,
                margin: EdgeInsets.only(
                  left: 16,
                  right: 16,
                  bottom: spacing,
                  top: spacing,
                ),
                underline: false,
                onChanged: (value) {
                  context.read<SettingBloc>().add(
                    SetMapPath(enabled: value ?? true),
                  );
                  context.read<ItemBloc>().add(const ItemStoreUpdated());
                },
              ),
              CheckBoxField(
                label: los.map_completed,
                value: state.mapCompleted,
                margin: EdgeInsets.only(
                  left: 16,
                  right: 16,
                  bottom: spacing,
                  top: spacing,
                ),
                underline: false,
                onChanged: (value) {
                  context.read<SettingBloc>().add(
                    SetMapCompleted(enabled: value ?? true),
                  );
                  context.read<ItemBloc>().add(const ItemStoreUpdated());
                },
              ),
              CheckBoxField(
                label: los.map_grid,
                value: state.mapGrid,
                margin: EdgeInsets.only(
                  left: 16,
                  right: 16,
                  bottom: spacing,
                  top: spacing,
                ),
                underline: false,
                onChanged: (value) {
                  context.read<SettingBloc>().add(
                    SetMapGrid(enabled: value ?? true),
                  );
                  context.read<ItemBloc>().add(const ItemStoreUpdated());
                },
              ),
              Padding(
                padding: EdgeInsets.only(
                  left: 16,
                  right: 16,
                  bottom: spacing,
                  top: 2,
                ),
                child: Row(
                  children: [
                    Text(
                      los.background,
                      style: TextStyle(
                        fontWeight: FontWeight.bold,
                        color: Theme.of(context).colorScheme.onSurfaceVariant,
                      ),
                    ),
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
                                    Theme.of(
                                      context,
                                    ).colorScheme.onSurfaceVariant,
                                width: 0.8,
                              ),
                            ),
                          ),
                        ),
                        items:
                            BorderBackground.values
                                .map(
                                  (e) => DropdownMenuItem<BorderBackground>(
                                    value: e,
                                    child: Text(
                                      _backgroundString(e, context.los),
                                      maxLines: 1,
                                      overflow: TextOverflow.ellipsis,
                                      softWrap: true,
                                    ),
                                  ),
                                )
                                .toList(),
                        onChanged: (value) {
                          final background = value ?? BorderBackground.color;
                          context.read<SettingBloc>().add(
                            SetBoundBackground(background: background),
                          );
                        },
                      ),
                    ),
                  ],
                ),
              ),
            ],
          ),
        );
      },
    );
  }
}
