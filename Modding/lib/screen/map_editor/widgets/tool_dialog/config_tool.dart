import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../../../extension/context.dart';
import '../../../../extension/platform.dart';
import '../../bloc/setting/setting_bloc.dart';
import '../../include/check_box_field.dart';

class ConfigSettingWidget extends StatefulWidget {
  const ConfigSettingWidget({super.key, required this.bloc});

  final SettingBloc bloc;

  @override
  State<StatefulWidget> createState() => _ConfigSettingState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<SettingBloc>('bloc', bloc));
  }
}

class _ConfigSettingState extends State<ConfigSettingWidget> {
  @override
  Widget build(BuildContext context) {
    final los = context.los;
    final bloc = widget.bloc;
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    final topSpacing = isDesktopPlatform ? 16.0 : 4.0;
    final bottomSpacing = isDesktopPlatform ? 8.0 : 2.0;
    return AlertDialog(
      title: Text(los.map_editor_config),
      content: SizedBox(
        width: 300,
        child: SingleChildScrollView(
          child: Column(
            children: [
              CheckBoxField(
                label: context.los.play_single_animation_frame,
                value: bloc.state.playSingleFrame,
                margin: EdgeInsets.only(
                  left: 16,
                  right: 16,
                  bottom: bottomSpacing,
                  top: topSpacing,
                ),
                underline: false,
                onChanged: ({required bool? value}) {
                  bloc.add(SetPlaySingleFrame(playSingleFrame: value ?? false));
                  setState(() {});
                },
              ),
              CheckBoxField(
                label: los.mute_audio,
                value: bloc.state.muteAudio,
                margin: EdgeInsets.only(
                  left: 16,
                  right: 16,
                  bottom: bottomSpacing,
                  top: topSpacing,
                ),
                underline: false,
                onChanged: ({required bool? value}) {
                  bloc.add(SetMuteAudio(muteAudio: value ?? false));
                  setState(() {});
                },
              ),
              CheckBoxField(
                label: los.plant_costume,
                value: bloc.state.plantCostume,
                margin: EdgeInsets.only(
                  left: 16,
                  right: 16,
                  bottom: bottomSpacing,
                  top: topSpacing,
                ),
                underline: false,
                onChanged: ({required bool? value}) {
                  bloc.add(SetPlantCostume(enabled: value ?? false));
                  setState(() {});
                },
              ),
              Padding(
                padding: EdgeInsets.only(
                  left: 16,
                  right: 16,
                  bottom: bottomSpacing,
                  top: topSpacing,
                ),
                child: Row(
                  children: [
                    Text(
                      los.filter_quality,
                      style: TextStyle(
                        fontWeight: FontWeight.bold,
                        color: Theme.of(context).colorScheme.onSurfaceVariant,
                      ),
                    ),
                    const Spacer(),
                    SizedBox(
                      width: 120.0,
                      child: _FilterQualityMenu(
                        value: bloc.state.filterQuality,
                        onChanged: (value) {
                          bloc.add(SetFilterQuality(filterQuality: value));
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
      actions: [
        TextButton(
          child: Text(los.okay),
          onPressed: () {
            Navigator.of(context).pop(true);
          },
        ),
      ],
    );
  }
}

class _FilterQualityMenu extends StatefulWidget {
  const _FilterQualityMenu({required this.value, required this.onChanged});
  final FilterQuality value;
  final void Function(FilterQuality) onChanged;

  @override
  State<_FilterQualityMenu> createState() => __FilterQualityMenuState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<void Function(FilterQuality p1)>.has(
        'onChanged',
        onChanged,
      ),
    );
    properties.add(EnumProperty<FilterQuality>('value', value));
  }
}

class __FilterQualityMenuState extends State<_FilterQualityMenu> {
  late final FocusNode _focusNode;
  late final TextEditingController _controller;
  late FilterQuality _selectedValue;

  @override
  void initState() {
    super.initState();
    _focusNode = FocusNode();
    _selectedValue = widget.value;
    _controller = TextEditingController(text: _selectedValue.name);
  }

  @override
  void dispose() {
    _focusNode.dispose();
    _controller.dispose();
    super.dispose();
  }

  void _selectItem(FilterQuality value) {
    setState(() {
      _selectedValue = value;
      _controller.text = value.name;
    });
    widget.onChanged(value);
  }

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(
      builder: (context, constraint) {
        return MenuAnchor(
          style: MenuStyle(
            minimumSize: WidgetStatePropertyAll(Size(constraint.maxWidth, 0)),
            maximumSize: WidgetStatePropertyAll(
              Size(constraint.maxWidth, double.infinity),
            ),
            elevation: WidgetStateProperty.all(4.0),
          ),
          crossAxisUnconstrained: false,
          alignmentOffset: const Offset(0, 8),
          menuChildren: [
            ...FilterQuality.values.map((e) {
              return MenuItemButton(
                onPressed: () {
                  _selectItem(e);
                  _focusNode.unfocus();
                },
                child: ListTile(
                  contentPadding: const EdgeInsets.symmetric(horizontal: 16.0),
                  title: Text(
                    e.name,
                    maxLines: 1,
                    overflow: TextOverflow.clip,
                    softWrap: true,
                    style: Theme.of(context).textTheme.bodyLarge?.copyWith(
                      color: Theme.of(context).colorScheme.onSurface,
                    ),
                  ),
                ),
              );
            }),
          ],
          builder: (context, controller, child) {
            return TextField(
              minLines: 1,
              maxLines: null,
              keyboardType: TextInputType.none,
              focusNode: _focusNode,
              readOnly: true,
              decoration: InputDecoration(
                contentPadding: const EdgeInsets.symmetric(
                  horizontal: 16.0,
                  vertical: 12.0,
                ),
                floatingLabelBehavior: FloatingLabelBehavior.auto,
                border: OutlineInputBorder(
                  borderSide: BorderSide(
                    color: Theme.of(context).colorScheme.onSurfaceVariant,
                    width: 0.8,
                  ),
                ),
                suffixIcon: Icon(
                  Symbols.arrow_drop_down,
                  color: Theme.of(context).colorScheme.onSurfaceVariant,
                ),
              ),
              controller: _controller,
              onTap: () {
                if (controller.isOpen) {
                  controller.close();
                } else {
                  controller.open();
                }
              },
              onTapOutside: (event) {
                _focusNode.unfocus();
              },
            );
          },
        );
      },
    );
  }
}
