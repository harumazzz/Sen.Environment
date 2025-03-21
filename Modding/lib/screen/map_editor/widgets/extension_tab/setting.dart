import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../../../../extension/context.dart';
import '../../../../extension/platform.dart';
import '../../bloc/item/item_bloc.dart';
import '../../bloc/section/section_bloc.dart';
import '../../bloc/setting/setting_bloc.dart';
import '../../include/check_box_field.dart';
import '../../models/config.dart';
import '../../../../i18n/app_localizations.dart';

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
                  onChanged: ({required bool? value}) {
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
                onChanged: ({required bool? value}) {
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
                onChanged: ({required bool? value}) {
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
                onChanged: ({required bool? value}) {
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
                onChanged: ({required bool? value}) {
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
                onChanged: ({required bool? value}) {
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
                onChanged: ({required bool? value}) {
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
                onChanged: ({required bool? value}) {
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
                onChanged: ({required bool? value}) {
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
                    const SizedBox(width: 130, child: _CustomAnchor()),
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

class _CustomAnchor extends StatefulWidget {
  const _CustomAnchor();

  @override
  __CustomAnchorState createState() => __CustomAnchorState();
}

class __CustomAnchorState extends State<_CustomAnchor> {
  late BorderBackground _selectedBackground;
  late FocusNode _focusNode;
  late TextEditingController _controller;

  @override
  void initState() {
    super.initState();
    _focusNode = FocusNode();
    _selectedBackground = context.read<SettingBloc>().state.boundBackground;
    _controller = TextEditingController();
    WidgetsBinding.instance.addPostFrameCallback((_) {
      _controller.text = _backgroundString(_selectedBackground, context.los);
    });
  }

  @override
  void dispose() {
    _focusNode.dispose();
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(
      builder: (context, constraint) {
        return MenuAnchor(
          crossAxisUnconstrained: false,
          alignmentOffset: const Offset(0, -48),
          childFocusNode: _focusNode,
          style: MenuStyle(
            minimumSize: WidgetStatePropertyAll(
              Size(constraint.maxWidth + 8, 0),
            ),
            maximumSize: WidgetStatePropertyAll(
              Size(constraint.maxWidth + 8, double.infinity),
            ),
          ),
          menuChildren: [
            ...BorderBackground.values.map((e) {
              return MenuItemButton(
                onPressed: () {
                  _selectBackground(e);
                  _focusNode.unfocus();
                },
                child: ConstrainedBox(
                  constraints: BoxConstraints(
                    maxWidth: constraint.maxWidth - 16,
                  ),
                  child: ListTile(
                    contentPadding: EdgeInsets.zero,
                    dense: true,
                    visualDensity: VisualDensity.standard,
                    title: Text(
                      _backgroundString(e, context.los),
                      overflow: TextOverflow.clip,
                      style: Theme.of(context).textTheme.bodyMedium,
                    ),
                  ),
                ),
              );
            }),
          ],
          builder: (context, controller, child) {
            return TextField(
              controller: _controller,
              focusNode: _focusNode,
              minLines: 1,
              maxLines: null,
              keyboardType: TextInputType.none,
              inputFormatters: const [],
              readOnly: true,
              decoration: InputDecoration(
                filled: false,
                border: const UnderlineInputBorder(),
                suffixIcon: IconButton(
                  icon: const Icon(Symbols.arrow_drop_down),
                  onPressed: () {
                    if (controller.isOpen) {
                      controller.close();
                    } else {
                      controller.open();
                    }
                  },
                ),
              ),
              onTapOutside: (e) {
                _focusNode.unfocus();
              },
            );
          },
        );
      },
    );
  }

  void _selectBackground(BorderBackground background) {
    setState(() {
      _selectedBackground = background;
      _controller.text = _backgroundString(background, context.los);
    });
    context.read<SettingBloc>().add(SetBoundBackground(background: background));
  }

  String _backgroundString(BorderBackground background, AppLocalizations los) {
    switch (background) {
      case BorderBackground.color:
        return los.color;
      case BorderBackground.timeSpace:
        return los.time_space;
    }
  }
}
