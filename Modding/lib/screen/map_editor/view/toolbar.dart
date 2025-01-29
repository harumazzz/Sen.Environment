import 'dart:collection';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/model/item.dart';
import 'package:sen/screen/map_editor/bloc/item/item_bloc.dart';
import 'package:sen/screen/map_editor/bloc/layer/layer_bloc.dart';
import 'package:sen/screen/map_editor/bloc/stage/stage_bloc.dart';
import 'package:sen/screen/map_editor/bloc/toolbar/toolbar_bloc.dart';

class ToolBarView extends StatelessWidget {
  const ToolBarView({super.key, required this.toolItem});

  final HashMap<ToolType, Item> toolItem;

  @override
  Widget build(BuildContext context) {
    final selectedColor = Theme.of(context).colorScheme.secondaryContainer;
    final buttonColor = Theme.of(context).colorScheme.surfaceContainerLow;
    final stageBloc = context.read<StageBloc>();
    final toolbarBloc = context.read<ToolBarBloc>();
    return BlocBuilder<ToolBarBloc, ToolBarState>(
        buildWhen: (prev, state) => prev.toolStatus != state.toolStatus,
        builder: (context1, state) {
          final toolStatus = state.toolStatus;
          return ListView.builder(
              itemCount: toolStatus.length,
              itemBuilder: (context, index) {
                final toolType = ToolType.values[index];
                final enabled = toolbarBloc.onEnabled(toolType);
                return ToolItem(
                    item: toolItem[toolType]!,
                    buttonColor: enabled ? selectedColor : buttonColor,
                    onSetting: () => switch (toolType) {
                          ToolType.openFile => toolbarBloc.add(ToolOpenEvent(
                              stageBloc: stageBloc,
                              itemBloc: context.read<ItemBloc>(),
                              layerBloc: context.read<LayerBloc>(),
                            )),
                          ToolType.saveFile => toolbarBloc.add(
                              ToolSaveEvent(
                                stageBloc: stageBloc,
                                // autosaveBloc: context.read<AutosaveBloc>(),
                              ),
                            ),
                          ToolType.clearEditor =>
                            toolbarBloc.add(const ToolClearEvent()),
                          ToolType.configEditor =>
                            toolbarBloc.add(const ToolConfigEvent()),
                          ToolType.shortcutMenu =>
                            toolbarBloc.add(const ShortcutMenuEvent()),
                          _ => toolbarBloc.add(ToolToogled(type: toolType))
                        });
              });
        });
  }
}

class ToolItem extends StatelessWidget {
  const ToolItem(
      {super.key,
      required this.item,
      required this.onSetting,
      required this.buttonColor});

  final Item item;

  final Color buttonColor;

  final void Function() onSetting;

  @override
  Widget build(BuildContext context) {
    return Tooltip(
      message: '${item.title}\n${item.description}.',
      waitDuration: const Duration(seconds: 1),
      child: Container(
          width: 45,
          height: 45,
          margin: const EdgeInsets.symmetric(vertical: 6, horizontal: 4),
          child: IconButton(
            style: ButtonStyle(
                backgroundColor: WidgetStateProperty.all(buttonColor),
                shape: WidgetStateProperty.all(RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(8)))),
            onPressed: onSetting,
            icon: item.icon,
          )),
    );
  }
}
