import 'dart:collection';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../../extension/platform.dart';
import '../../../model/item.dart';
import '../bloc/section/section_bloc.dart';

class SectionView extends StatelessWidget {
  const SectionView({super.key, required this.sectionItem});

  final HashMap<SectionType, Item> sectionItem;

  @override
  Widget build(BuildContext context) {
    final selectedColor = Theme.of(context).colorScheme.secondaryContainer;
    final buttonColor = Theme.of(context).colorScheme.surfaceBright;
    final minizeColor = Theme.of(context).colorScheme.surfaceContainerHighest;
    final isLowScreenWidth = MediaQuery.sizeOf(context).width < 1340;
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    return BlocBuilder<SectionBloc, SectionState>(
      buildWhen:
          (prev, state) =>
              prev.section != state.section ||
              prev.sectionMinize != state.sectionMinize,
      builder: (context, state) {
        final sectionType = state.section;
        if (isDesktopPlatform) {
          return ListView.builder(
            scrollDirection: Axis.horizontal,
            itemCount: sectionItem.length,
            itemBuilder: (context, index) {
              final type = SectionType.values[index];
              final isMinize = state.sectionMinize[type]!;
              return SectionItem(
                selected: !isLowScreenWidth || sectionType == type,
                item: sectionItem[type]!,
                buttonColor:
                    sectionType == type
                        ? isMinize
                            ? minizeColor
                            : selectedColor
                        : buttonColor,
                onSetting:
                    () =>
                        type == sectionType
                            ? context.read<SectionBloc>().add(
                              SectionMinizeToggled(
                                type: type,
                                minize: !isMinize,
                              ),
                            )
                            : context.read<SectionBloc>().add(
                              SetSection(section: type),
                            ),
              );
            },
          );
        } else {
          return Row(
            children:
                SectionType.values.map((type) {
                  final isMinize = state.sectionMinize[type]!;
                  return SizedBox(
                    height: 60,
                    child: SectionItem(
                      selected: !isLowScreenWidth || sectionType == type,
                      item: sectionItem[type]!,
                      buttonColor:
                          sectionType == type
                              ? isMinize
                                  ? minizeColor
                                  : selectedColor
                              : buttonColor,
                      onSetting:
                          () =>
                              isMinize && type == sectionType
                                  ? context.read<SectionBloc>().add(
                                    SectionMinizeToggled(
                                      type: type,
                                      minize: false,
                                    ),
                                  )
                                  : context.read<SectionBloc>().add(
                                    SetSection(section: type),
                                  ),
                    ),
                  );
                }).toList(),
          );
        }
      },
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<HashMap<SectionType, Item>>(
        'sectionItem',
        sectionItem,
      ),
    );
  }
}

class SectionItem extends StatelessWidget {
  const SectionItem({
    super.key,
    required this.selected,
    required this.item,
    required this.buttonColor,
    required this.onSetting,
  });

  final bool selected;

  final Item item;

  final Color buttonColor;

  final void Function() onSetting;

  @override
  Widget build(BuildContext context) {
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    final toolWidth =
        isDesktopPlatform ? 45.0 : MediaQuery.sizeOf(context).width / 9;
    return Tooltip(
      message: '${item.description}.',
      waitDuration: const Duration(seconds: 1),
      child: Card(
        margin:
            isDesktopPlatform
                ? const EdgeInsets.only(top: 5, bottom: 8, left: 10)
                : null,
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(8)),
        elevation: isDesktopPlatform ? null : 0,
        color: buttonColor,
        child: InkWell(
          borderRadius: BorderRadius.circular(8),
          onTap: onSetting,
          child: SizedBox(
            width: selected ? 180 : toolWidth,
            child:
                selected
                    ? Row(
                      children: [
                        Container(
                          margin: const EdgeInsets.symmetric(horizontal: 10),
                          child: Icon(item.icon),
                        ),
                        Text(
                          item.title,
                          style: const TextStyle(fontWeight: FontWeight.bold),
                        ),
                      ],
                    )
                    : Container(
                      margin: const EdgeInsets.symmetric(horizontal: 10),
                      child: Icon(item.icon),
                    ),
          ),
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<bool>('selected', selected));
    properties.add(DiagnosticsProperty<Item>('item', item));
    properties.add(ColorProperty('buttonColor', buttonColor));
    properties.add(
      ObjectFlagProperty<void Function()>.has('onSetting', onSetting),
    );
  }
}

class ExtensionView extends StatelessWidget {
  const ExtensionView({super.key, required this.extensionItem});

  final HashMap<ExtensionType, Item> extensionItem;

  @override
  Widget build(BuildContext context) {
    final selectedColor = Theme.of(context).colorScheme.secondaryContainer;
    final buttonColor = Theme.of(context).colorScheme.surfaceBright;
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    return BlocBuilder<SectionBloc, SectionState>(
      buildWhen: (prev, state) => prev.extension != state.extension,
      builder: (context, state) {
        if (isDesktopPlatform) {
          return ListView.builder(
            scrollDirection: Axis.horizontal,
            itemCount: extensionItem.length,
            itemBuilder: (context, index) {
              final type = ExtensionType.values[index];
              final enabled = context.read<SectionBloc>().onExtensionEnabled(
                type,
              );
              return ExtensionItem(
                item: extensionItem[type]!,
                buttonColor: enabled ? selectedColor : buttonColor,
                onSetting:
                    () => context.read<SectionBloc>().add(
                      ExtensionToggled(type: type),
                    ),
              );
            },
          );
        } else {
          return Row(
            children:
                ExtensionType.values.map((type) {
                  final enabled = context
                      .read<SectionBloc>()
                      .onExtensionEnabled(type);
                  return SizedBox(
                    height: 60,
                    child: ExtensionItem(
                      item: extensionItem[type]!,
                      buttonColor: enabled ? selectedColor : buttonColor,
                      onSetting:
                          () => context.read<SectionBloc>().add(
                            ExtensionToggled(type: type),
                          ),
                    ),
                  );
                }).toList(),
          );
        }
      },
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<HashMap<ExtensionType, Item>>(
        'extensionItem',
        extensionItem,
      ),
    );
  }
}

class ExtensionItem extends StatelessWidget {
  const ExtensionItem({
    super.key,
    required this.item,
    required this.buttonColor,
    required this.onSetting,
  });

  final Item item;

  final Color buttonColor;

  final void Function() onSetting;

  @override
  Widget build(BuildContext context) {
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    final toolWidth =
        isDesktopPlatform ? 45.0 : MediaQuery.sizeOf(context).width / 9;
    return Tooltip(
      message: '${item.title}\n${item.description}.',
      waitDuration: const Duration(seconds: 1),
      child: Card(
        elevation: isDesktopPlatform ? null : 0,
        margin:
            isDesktopPlatform
                ? const EdgeInsets.only(top: 5, bottom: 8, left: 10)
                : null,
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(8)),
        color: buttonColor,
        child: InkWell(
          borderRadius: BorderRadius.circular(8),
          onTap: onSetting,
          child: SizedBox(width: toolWidth, child: Icon(item.icon)),
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Item>('item', item));
    properties.add(ColorProperty('buttonColor', buttonColor));
    properties.add(
      ObjectFlagProperty<void Function()>.has('onSetting', onSetting),
    );
  }
}
