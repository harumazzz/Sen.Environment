import 'dart:collection';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/model/item.dart';
import 'package:sen/screen/map_editor/bloc/section/section_bloc.dart';

class SectionView extends StatelessWidget {
  const SectionView({super.key, required this.sectionItem});

  final HashMap<SectionType, Item> sectionItem;

  @override
  Widget build(BuildContext context) {
    final selectedColor = Theme.of(context).colorScheme.secondaryContainer;
    final buttonColor = Theme.of(context).colorScheme.surfaceBright;
    final minizeColor = Theme.of(context).colorScheme.surfaceContainerHighest;
    return BlocBuilder<SectionBloc, SectionState>(
        buildWhen: (prev, state) => prev.section != state.section || prev.sectionMinize != state.sectionMinize,
        builder: (context, state) {
          final sectionType = state.section;
          return ListView.builder(
              scrollDirection: Axis.horizontal,
              itemCount: sectionItem.length,
              itemBuilder: (context, index) {
                final type = SectionType.values[index];
                final isMinize = state.sectionMinize[type]!;
                return SectionItem(
                  item: sectionItem[type]!,
                  buttonColor:
                      sectionType == type ? isMinize ? minizeColor : selectedColor : buttonColor,
                  onSetting: () => isMinize && type == sectionType ? context
                      .read<SectionBloc>()
                      .add(SectionMinizeToggled(type: type, minize: false)) : context
                      .read<SectionBloc>()
                      .add(SetSection(section: type)),
                );
              });
        });
  }
}

class SectionItem extends StatelessWidget {
  const SectionItem(
      {super.key,
      required this.item,
      required this.buttonColor,
      required this.onSetting});

  final Item item;

  final Color buttonColor;

  final void Function() onSetting;

  @override
  Widget build(BuildContext context) {
    return Tooltip(
      message: '${item.description}.',
      waitDuration: const Duration(seconds: 1),
      child: Card(
          margin: const EdgeInsets.only(top: 5, bottom: 8, left: 10),
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(8),
          ),
          color: buttonColor,
          child: InkWell(
            borderRadius: BorderRadius.circular(8),
            onTap: onSetting,
            child: SizedBox(
              width: 180,
              child: Row(
                children: [
                  Container(
                      margin: const EdgeInsets.symmetric(horizontal: 10),
                      child: item.icon),
                  Text(
                    item.title,
                    style: const TextStyle(fontWeight: FontWeight.bold),
                  )
                ],
              ),
            ),
          )),
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
    return BlocBuilder<SectionBloc, SectionState>(
        buildWhen: (prev, state) => prev.extension != state.extension,
        builder: (context, state) {
          return ListView.builder(
              scrollDirection: Axis.horizontal,
              itemCount: extensionItem.length,
              itemBuilder: (context, index) {
                final type = ExtensionType.values[index];
                final enabled =
                    context.read<SectionBloc>().onExtensionEnabled(type);
                return ExtensionItem(
                  item: extensionItem[type]!,
                  buttonColor: enabled ? selectedColor : buttonColor,
                  onSetting: () => context
                      .read<SectionBloc>()
                      .add(ExtensionToggled(type: type)),
                );
              });
        });
  }
}

class ExtensionItem extends StatelessWidget {
  const ExtensionItem(
      {super.key,
      required this.item,
      required this.buttonColor,
      required this.onSetting});

  final Item item;

  final Color buttonColor;

  final void Function() onSetting;

  @override
  Widget build(BuildContext context) {
    return Tooltip(
      message: '${item.title}\n${item.description}.',
      waitDuration: const Duration(seconds: 1),
      child: Card(
          margin: const EdgeInsets.only(top: 5, bottom: 8, left: 10),
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(8),
          ),
          color: buttonColor,
          child: InkWell(
            borderRadius: BorderRadius.circular(8),
            onTap: onSetting,
            child: SizedBox(
              width: 45,
              child: item.icon,
            ),
          )),
    );
  }
}
