import 'package:flex_color_picker/flex_color_picker.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../../bloc/section/section_bloc.dart';
import '../../bloc/setting/setting_bloc.dart';

class PaletteWidget extends StatelessWidget {
  const PaletteWidget({super.key});

  @override
  Widget build(BuildContext context) {
    final item =
        context
            .read<MapEditorConfigurationCubit>()
            .state
            .extensionItem[ExtensionType.palette]!;
    return SizedBox(
      width: 300,
      height: 445,
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
                            type: ExtensionType.palette,
                            enabled: false,
                          ),
                        );
                      },
                      icon: const Icon(Symbols.close),
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
                  child: ColorPicker(
                    wheelDiameter: 172,
                    color: context.read<SettingBloc>().state.boundingColor,
                    onColorChanged: (color) {
                      context.read<SettingBloc>().add(
                        SetBoundingColor(color: color),
                      );
                    },
                    pickersEnabled: const <ColorPickerType, bool>{
                      ColorPickerType.both: false,
                      ColorPickerType.primary: true,
                      ColorPickerType.accent: false,
                      ColorPickerType.bw: false,
                      ColorPickerType.custom: false,
                      ColorPickerType.wheel: true,
                    },
                    showColorName: true,
                  ),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
