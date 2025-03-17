import 'dart:collection';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../../cubit/initial_directory_cubit/initial_directory_cubit.dart';
import '../../../extension/platform.dart';
import '../../../model/item.dart';
import '../bloc/item/item_bloc.dart';
import '../bloc/layer/layer_bloc.dart';
import '../bloc/stage/stage_bloc.dart';
import '../bloc/toolbar/toolbar_bloc.dart';

class ToolBarView extends StatelessWidget {
  const ToolBarView({super.key, required this.toolItem});

  final HashMap<ToolType, Item> toolItem;

  @override
  Widget build(BuildContext context) {
    final stageBloc = context.read<StageBloc>();
    final toolbarBloc = context.read<ToolBarBloc>();
    final isDesktopPlatform = CurrentPlatform.isDesktop;

    final selectedColor = Theme.of(context).colorScheme.secondaryContainer;
    final buttonColor = Theme.of(context).colorScheme.surfaceContainerLow;
    return BlocBuilder<ToolBarBloc, ToolBarState>(
      buildWhen: (prev, state) => prev.toolStatus != state.toolStatus,
      builder: (context1, state) {
        final toolStatus = state.toolStatus;
        if (!isDesktopPlatform) {
          toolStatus.remove(ToolType.shortcutMenu);
        }
        return ListView.builder(
          scrollDirection: isDesktopPlatform ? Axis.vertical : Axis.horizontal,
          itemCount: toolStatus.length,
          itemBuilder: (context, index) {
            final toolType = ToolType.values[index];
            final enabled = toolbarBloc.onEnabled(toolType);
            return ToolItem(
              item: toolItem[toolType]!,
              buttonColor: enabled ? selectedColor : buttonColor,
              onSetting:
                  () => switch (toolType) {
                    ToolType.openFile => toolbarBloc.add(
                      ToolOpenEvent(
                        stageBloc: stageBloc,
                        itemBloc: context.read<ItemBloc>(),
                        layerBloc: context.read<LayerBloc>(),
                        initialDirectoryCubit:
                            BlocProvider.of<InitialDirectoryCubit>(context),
                      ),
                    ),
                    ToolType.saveFile => toolbarBloc.add(
                      ToolSaveEvent(
                        stageBloc: stageBloc,
                        initialDirectoryCubit:
                            context.read<InitialDirectoryCubit>(),
                        // autosaveBloc: context.read<AutosaveBloc>(),
                      ),
                    ),
                    ToolType.clearEditor => toolbarBloc.add(
                      const ToolClearEvent(),
                    ),
                    ToolType.configEditor => toolbarBloc.add(
                      const ToolConfigEvent(),
                    ),
                    ToolType.shortcutMenu => toolbarBloc.add(
                      const ShortcutMenuEvent(),
                    ),
                    _ => toolbarBloc.add(ToolToogled(type: toolType)),
                  },
            );
          },
        );
      },
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<HashMap<ToolType, Item>>('toolItem', toolItem),
    );
  }
}

class ToolItem extends StatelessWidget {
  const ToolItem({
    super.key,
    required this.item,
    required this.onSetting,
    required this.buttonColor,
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
      child: Container(
        width: toolWidth,
        height: 45,
        margin: const EdgeInsets.symmetric(vertical: 6, horizontal: 4),
        child: IconButton(
          style: ButtonStyle(
            backgroundColor: WidgetStateProperty.all(buttonColor),
            shape: WidgetStateProperty.all(
              RoundedRectangleBorder(borderRadius: BorderRadius.circular(8.0)),
            ),
          ),
          onPressed: onSetting,
          icon: Icon(item.icon),
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
