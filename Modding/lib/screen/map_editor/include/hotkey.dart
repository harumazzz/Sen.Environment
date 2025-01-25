import 'dart:collection';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/model/worldmap.dart';
import 'package:sen/screen/map_editor/bloc/autosave/autosave_bloc.dart';
import 'package:sen/screen/map_editor/bloc/history/history_bloc.dart';
import 'package:sen/screen/map_editor/bloc/item/item_bloc.dart';
import 'package:sen/screen/map_editor/bloc/section/section_bloc.dart';
import 'package:sen/screen/map_editor/bloc/selected/selected_bloc.dart';
import 'package:sen/screen/map_editor/bloc/stage/stage_bloc.dart';
import 'package:sen/screen/map_editor/bloc/toolbar/toolbar_bloc.dart';
import 'package:sen/screen/map_editor/include/controller.dart';
import 'package:sen/screen/map_editor/include/hotkey_listener.dart';
import 'package:sen/screen/map_editor/bloc/layer/layer_bloc.dart';
import 'package:sen/screen/map_editor/models/action_model.dart';
import 'package:sen/screen/map_editor/models/action_service.dart';

class Hotkey extends StatelessWidget {
  const Hotkey({super.key, required this.controller, required this.child});

  final Widget child;

  final CanvasController controller;

  void _delete(BuildContext context) {
    final stageBloc = context.read<StageBloc>();
    final layerBloc = context.read<LayerBloc>();
    final selectedList = context.read<SelectedBloc>().state.selectedList;
    context.read<StageBloc>().add(
          RemoveItemEvent(
            idList: selectedList,
            itemBloc: context.read<ItemBloc>(),
            layerBloc: context.read<LayerBloc>(),
          ),
        );
    final actionService = ActionService<ActionModelType>(
      actionType: ActionType.eraseItem,
      data: {
        ActionModelType.mapPieces: stageBloc.clonePieces(),
        ActionModelType.mapEvents: stageBloc.cloneEvents(),
      },
      change: (data) {
        final pieces = data![ActionModelType.mapPieces] as HashMap<String, MapPieceItem>;
        final events = data[ActionModelType.mapEvents] as HashMap<String, MapEventItem>;
        stageBloc.add(UpdateStageState(stageState: stageBloc.state.copyWith(events: events, pieces: pieces)));
        final layerKeys = <int>[];
        for (final e in pieces.values) {
          if (!layerKeys.contains(e.layer)) {
            layerKeys.add(e.layer);
          }
        }
        for (final layerId in layerKeys) {
          layerBloc.updateNodeParallax(layerId, pieceItems: pieces);
        }
        layerBloc.updateTree(true);
      },
    );
    context.read<HistoryBloc>().add(CaptureState(state: actionService));
  }

  void _copy(BuildContext context) {
    controller.catchPointerHover();
    context.read<SelectedBloc>().add(const CopySelectedList());
  }

  @override
  Widget build(BuildContext context) {
    final toolbarBloc = context.read<ToolBarBloc>();
    final stageBloc = context.read<StageBloc>();
    final layerBloc = context.read<LayerBloc>();
    return HotkeyListener(
        controller: controller,
        onKeyDownEvent: (logicalKey) {
          if (controller.controlPressed || controller.metaPressed) {
            switch (logicalKey) {
              case LogicalKeyboardKey.keyC:
                {
                  _copy(context);
                  break;
                }
              case LogicalKeyboardKey.keyV:
                {
                  final copyList = context.read<SelectedBloc>().state.copyList;
                  final details = controller.getDetailsMove();
                  if (copyList.isNotEmpty) {
                    context.read<StageBloc>().add(CloneItemEvent(
                        idList: copyList,
                        itemBloc: context.read<ItemBloc>(),
                        layerBloc: context.read<LayerBloc>(),
                        positionXAdditional: details.dx,
                        positionYAdditional: details.dy));

                    final actionService = ActionService<ActionModelType>(
                        actionType: ActionType.pasteItem,
                        data: {
                          ActionModelType.mapPieces: stageBloc.clonePieces(),
                          ActionModelType.mapEvents: stageBloc.cloneEvents(),
                        },
                        change: (data) {
                          final pieces = data![ActionModelType.mapPieces] as HashMap<String, MapPieceItem>;
                          final events = data[ActionModelType.mapEvents] as HashMap<String, MapEventItem>;
                          stageBloc.add(
                              UpdateStageState(stageState: stageBloc.state.copyWith(events: events, pieces: pieces)));
                          final layerKeys = <int>[];
                          for (final e in pieces.values) {
                            if (!layerKeys.contains(e.layer)) {
                              layerKeys.add(e.layer);
                            }
                          }
                          for (final layerId in layerKeys) {
                            layerBloc.updateNodeParallax(layerId, pieceItems: pieces);
                          }
                          layerBloc.updateTree(true);
                          // historyBloc.add(const UpdateIndexEvent());
                        });
                    context.read<HistoryBloc>().add(CaptureState(state: actionService));
                  }
                  break;
                }
              default:
                {
                  toolbarBloc.add(const ToolToogled(type: ToolType.rectangleTool, enabled: true));
                  break;
                }
            }
          } else {
            switch (logicalKey) {
              case LogicalKeyboardKey.keyE:
                {
                  toolbarBloc.add(const ToolToogled(type: ToolType.eraseTool, enabled: true));
                  break;
                }
              case LogicalKeyboardKey.space:
                {
                  toolbarBloc.add(const ToolToogled(type: ToolType.panTool, enabled: true));
                  break;
                }
              default:
                break;
            }
          }
        },
        onKeyUpEvent: (logicalKey) {
          if (controller.controlPressed || controller.metaPressed) {
            switch (logicalKey) {
              case LogicalKeyboardKey.keyO:
                {
                  toolbarBloc.add(ToolOpenEvent(
                    stageBloc: context.read<StageBloc>(),
                    itemBloc: context.read<ItemBloc>(),
                    layerBloc: context.read<LayerBloc>(),
                  ));
                }
              case LogicalKeyboardKey.keyS:
                {
                  toolbarBloc.add(ToolSaveEvent(
                    stageBloc: context.read<StageBloc>(),
                    // autosaveBloc: context.read<AutosaveBloc>(),
                  ));
                  break;
                }
              case LogicalKeyboardKey.keyR:
                {
                  toolbarBloc.add(const ToolToogled(type: ToolType.resizeTool));
                  break;
                }
              case LogicalKeyboardKey.keyL:
                {
                  context.read<SectionBloc>().add(const ExtensionToggled(
                        type: ExtensionType.layer,
                      ));
                  break;
                }
              case LogicalKeyboardKey.keyH:
                {
                  context.read<SectionBloc>().add(const ExtensionToggled(
                        type: ExtensionType.history,
                      ));
                  break;
                }
              case LogicalKeyboardKey.keyI:
                {
                  context.read<SectionBloc>().add(const ExtensionToggled(
                        type: ExtensionType.setting,
                      ));
                  break;
                }
              case LogicalKeyboardKey.keyP:
                {
                  context.read<SectionBloc>().add(const ExtensionToggled(
                        type: ExtensionType.palette,
                      ));
                  break;
                }
              case LogicalKeyboardKey.keyZ:
                {
                  final historyBloc = context.read<HistoryBloc>();
                  if (historyBloc.state.captureManager.canUndo()) {
                    historyBloc.add(const UndoEvent());
                    final scrollController = historyBloc.state.scrollController;
                    if (!(scrollController.position.atEdge && scrollController.position.pixels == 0)) {
                      scrollController.jumpTo(scrollController.position.pixels - 45);
                    }
                  }
                  break;
                }
              case LogicalKeyboardKey.keyY:
                {
                  final historyBloc = context.read<HistoryBloc>();
                  if (historyBloc.state.captureManager.canRedo()) {
                    historyBloc.add(const RedoEvent());
                    final scrollController = historyBloc.state.scrollController;
                    if (!(scrollController.position.atEdge && scrollController.position.pixels != 0)) {
                      scrollController.jumpTo(scrollController.position.pixels + 45);
                    }
                  }
                  break;
                }
              default:
                {
                  toolbarBloc.add(const ToolToogled(type: ToolType.rectangleTool, enabled: false));
                  break;
                }
            }
          } else {
            switch (logicalKey) {
              case LogicalKeyboardKey.f2:
                {
                  toolbarBloc.add(const ShortcutMenuEvent());
                }
              case LogicalKeyboardKey.f8:
                {
                  toolbarBloc.add(const ToolClearEvent());
                  break;
                }
              case LogicalKeyboardKey.f9:
                {
                  toolbarBloc.add(const ToolConfigEvent());
                  break;
                }
              case LogicalKeyboardKey.delete:
                {
                  _delete(context);
                  break;
                }
              case LogicalKeyboardKey.keyE:
                {
                  toolbarBloc.add(const ToolToogled(type: ToolType.eraseTool, enabled: false));
                  break;
                }
              case LogicalKeyboardKey.space:
                {
                  toolbarBloc.add(const ToolToogled(type: ToolType.panTool, enabled: false));
                  break;
                }
              default:
                break;
            }
          }
        },
        child: child);
  }
}
