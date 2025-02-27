import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:nil/nil.dart';
import 'package:pie_menu/pie_menu.dart';
import 'package:sen/cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'package:sen/extension/platform.dart';
import 'package:sen/screen/map_editor/bloc/canvas/canvas_bloc.dart';
import 'package:sen/screen/map_editor/bloc/item/item_bloc.dart';
import 'package:sen/screen/map_editor/bloc/mouse_cursor/mouse_cursor_bloc.dart';
import 'package:sen/screen/map_editor/bloc/selected/selected_bloc.dart';
import 'package:sen/screen/map_editor/bloc/setting/setting_bloc.dart';
import 'package:sen/screen/map_editor/bloc/shortcut/shortcut_bloc.dart';
import 'package:sen/screen/map_editor/bloc/toolbar/toolbar_bloc.dart';
import 'package:sen/screen/map_editor/include/hotkey.dart';
import 'package:sen/screen/map_editor/include/hover_box.dart';
import 'package:sen/screen/map_editor/include/mouse_listener.dart';
import 'package:sen/screen/map_editor/include/painter.dart';
import 'package:sen/screen/map_editor/include/selected_box.dart';
import 'package:sen/screen/map_editor/widgets/box_stage.dart';

class MapStageView extends StatelessWidget {
  const MapStageView({super.key});

  @override
  Widget build(BuildContext context) {
    return Builder(builder: (context) {
      final settingState = context.watch<SettingBloc>().state;
      final itemState = context.watch<ItemBloc>().state;
      final itemStore = itemState.itemStore;
      final stackList = <Widget>[];
      for (final id in itemStore.keys) {
        stackList.add(itemStore[id]!.widget!);
      }
      final editorResource = context.read<MapEditorConfigurationCubit>().state.editorResource;
      final controller = context.read<CanvasBloc>().state.canvasController;
      final isDesktopPlatform = CurrentPlatform.isDesktop;
      return BlocBuilder<SelectedBloc, SelectedState>(
          buildWhen: (prev, state) => prev.copyList != state.copyList,
          builder: (context, selectedState) {
            return BlocBuilder<MouseCursorBloc, MouseCursorState>(
                buildWhen: (prev, state) => prev.cursor != state.cursor,
                builder: (context, mouseState) {
                  return Hotkey(
                      controller: controller,
                      child: BlocBuilder<ToolBarBloc, ToolBarState>(
                          buildWhen: (prev, state) => prev.toolStatus != state.toolStatus,
                          builder: (context, toolState) {
                            final panTool = toolState.toolStatus[ToolType.panTool]!;
                            final resize = toolState.toolStatus[ToolType.resizeTool]!;
                            final multiSelect = toolState.toolStatus[ToolType.rectangleTool]!;
                            final eraseTool = toolState.toolStatus[ToolType.eraseTool]!;
                            return MouseListener(
                                itemStore: itemStore,
                                multiSelect: multiSelect,
                                eraseTool: eraseTool,
                                panTool: panTool,
                                resize: resize,
                                builder: (context, state) {
                                  final onSelectedId = state.onSelect;
                                  final selectedList = state.selectedList;
                                  final hoverSelected = state.hoverSelect;
                                  if (panTool && isDesktopPlatform) {
                                    context.read<MouseCursorBloc>().add(
                                          ChangeCursorEvent(
                                            cursor: editorResource.panCursor!,
                                          ),
                                        );
                                  } else if (eraseTool && isDesktopPlatform) {
                                    context
                                        .read<MouseCursorBloc>()
                                        .add(ChangeCursorEvent(cursor: editorResource.eraseCursor!));
                                  } else if (multiSelect && isDesktopPlatform) {
                                    context
                                        .read<MouseCursorBloc>()
                                        .add(ChangeCursorEvent(cursor: editorResource.multiSelectCursor!));
                                  } else {
                                    if (state.onSelect == null) {
                                      context
                                          .read<MouseCursorBloc>()
                                          .add(const ChangeCursorEvent(cursor: SystemMouseCursors.basic));
                                    }
                                  }
                                  final shortcutBloc = context.read<ShortcutBloc>();

                                  return MouseRegion(
                                      cursor: mouseState.cursor,
                                      onHover: (details) {
                                        controller.listenerPointerHover(details);
                                      },
                                      child: BoxStage(
                                          mapGrid: settingState.mapGrid,
                                          usePanTool: panTool,
                                          useEraseTool: eraseTool,
                                          useResizeTool: resize,
                                          boundBackground: settingState.boundBackground,
                                          boxStageColor: settingState.boundingColor,
                                          pieMenuAction: <PieAction>[
                                            if (selectedList.isNotEmpty && hoverSelected)
                                              PieAction(
                                                  tooltip: nil,
                                                  onSelect: () {
                                                    shortcutBloc.cut(controller);
                                                  },
                                                  child: const Icon(Icons.content_cut)),
                                            if (selectedList.isNotEmpty && hoverSelected)
                                              PieAction(
                                                  tooltip: nil,
                                                  onSelect: () {
                                                    shortcutBloc.copy(controller);
                                                  },
                                                  child: const Icon(Symbols.content_copy)),
                                            if (selectedList.isNotEmpty && hoverSelected)
                                              PieAction(
                                                  tooltip: nil,
                                                  onSelect: () {
                                                    shortcutBloc.delete();
                                                  },
                                                  child: const Icon(Symbols.delete)),
                                            if (state.copyList.isNotEmpty)
                                              PieAction(
                                                  tooltip: nil,
                                                  onSelect: () {
                                                    shortcutBloc.paste(controller);
                                                  },
                                                  child: const Icon(Symbols.content_paste))
                                          ],
                                          children: [
                                            ...stackList,
                                            /*
                                            Positioned(
                                              top: MapConst.debugOffset.dy,
                                              left: MapConst.debugOffset.dx,
                                              child: const ColoredBox(color: Colors.white, child: SizedBox(width: 30, height: 30,))),
                                              */
                                            if (!panTool &&
                                                !resize &&
                                                onSelectedId != null &&
                                                itemStore.containsKey(onSelectedId) &&
                                                !selectedList.contains(onSelectedId))
                                              HoverBox(
                                                matrix: itemStore[onSelectedId]!.matrix!,
                                                rect: itemStore[onSelectedId]!.itemRect ?? Rect.zero,
                                              ),
                                            if (!panTool && selectedList.isNotEmpty)
                                              MultiSelectBox(
                                                idList: selectedList.toList(),
                                                itemStore: itemStore,
                                              ),
                                            if (controller.marqueeStart != null &&
                                                controller.marqueeEnd != null &&
                                                multiSelect)
                                              Positioned.fill(
                                                child: Marquee(
                                                  start: controller.toLocal(controller.marqueeStart!),
                                                  end: controller.toLocal(controller.marqueeEnd!),
                                                ),
                                              ),
                                          ]));
                                });
                          }));
                });
          });
    });
  }
}
