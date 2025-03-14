import 'dart:collection';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../../extension/platform.dart';
import '../../../model/worldmap.dart';
import '../bloc/canvas/canvas_bloc.dart';
import '../bloc/history/history_bloc.dart';
import '../bloc/item/item_bloc.dart';
import '../bloc/layer/layer_bloc.dart';
import '../bloc/mouse_cursor/mouse_cursor_bloc.dart';
import '../bloc/selected/selected_bloc.dart';
import '../bloc/stage/stage_bloc.dart';
import '../models/action_model.dart';
import '../models/action_service.dart';
import '../models/item_profile.dart';

class MouseListener extends StatelessWidget {
  const MouseListener({
    super.key,
    required this.itemStore,
    required this.multiSelect,
    required this.eraseTool,
    required this.panTool,
    required this.resize,
    required this.builder,
  });

  final Map<String, ItemProfile> itemStore;

  final bool multiSelect;

  final bool eraseTool;

  final bool panTool;

  final bool resize;

  final BlocWidgetBuilder<SelectedState> builder;

  void _hoverListener(
    BuildContext context,
    Offset details,
    Iterable<String> selectedList,
  ) {
    final idList = itemStore.keys.toList();
    final controller = context.read<CanvasBloc>().state.canvasController;
    final controllerTransform = controller.transformationController.value;
    final viewportScale = controllerTransform.getMaxScaleOnAxis();
    final sectionRect = Rect.fromLTWH(
      (details.dx - controllerTransform[12]) / viewportScale,
      (details.dy - controllerTransform[13]) / viewportScale,
      3,
      3,
    );
    //MapConst.debugOffset = sectionRect.topLeft;
    for (var i = idList.length - 1; i >= 0; --i) {
      final id = idList[i];
      final itemProfile = itemStore[id]!;
      final itemRect = Rect.fromLTWH(
        itemProfile.selectRect!.left,
        itemProfile.selectRect!.top,
        itemProfile.selectRect!.width / viewportScale,
        itemProfile.selectRect!.height / viewportScale,
      );
      final selected = sectionRect.overlaps(itemRect);
      if (selected) {
        if (!panTool && !eraseTool && !multiSelect) {
          if (selectedList.contains(id)) {
            context.read<MouseCursorBloc>().add(
              const ChangeCursorEvent(cursor: SystemMouseCursors.allScroll),
            );
          } else {
            context.read<MouseCursorBloc>().add(
              const ChangeCursorEvent(cursor: SystemMouseCursors.click),
            );
          }
          context.read<SelectedBloc>().add(
            const SetHoverSelected(selected: true),
          );
        }
        context.read<SelectedBloc>().add(OnSelectUpdated(onSelect: id));
        return;
      } else {
        if (!panTool && !eraseTool && !multiSelect) {
          context.read<MouseCursorBloc>().add(
            const ChangeCursorEvent(cursor: SystemMouseCursors.basic),
          );
        }
      }
    }
    context.read<SelectedBloc>().add(const SetOnSelectedNull());
    return;
  }

  @override
  Widget build(BuildContext context) {
    final idList = itemStore.keys.toList();
    final enable = !panTool && !resize;
    final stageBloc = context.read<StageBloc>();
    final itemBloc = context.read<ItemBloc>();
    final historyBloc = context.read<HistoryBloc>();
    final layerBloc = context.read<LayerBloc>();
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    return BlocBuilder<SelectedBloc, SelectedState>(
      builder: (context, selectedState) {
        var onSelectedId = selectedState.onSelect;
        final selectedList = selectedState.selectedList;
        final controller = context.read<CanvasBloc>().state.canvasController;
        return Listener(
          onPointerHover: (details) {
            controller.requestFocus();
            if (enable && isDesktopPlatform) {
              _hoverListener(context, details.localPosition, selectedList);
            }
          },
          onPointerDown: (details) {
            if (enable) {
              if (!isDesktopPlatform) {
                final controllerTransform =
                    controller.transformationController.value;
                final viewportScale = controllerTransform.getMaxScaleOnAxis();
                final sectionRect = Rect.fromLTWH(
                  (details.localPosition.dx - controllerTransform[12]) /
                      viewportScale,
                  (details.localPosition.dy - controllerTransform[13]) /
                      viewportScale,
                  3,
                  3,
                );
                //MapConst.debugOffset = sectionRect.topLeft;
                for (var i = idList.length - 1; i >= 0; --i) {
                  final id = idList[i];
                  final itemProfile = itemStore[id]!;
                  final itemRect = Rect.fromLTWH(
                    itemProfile.selectRect!.left,
                    itemProfile.selectRect!.top,
                    itemProfile.selectRect!.width / viewportScale,
                    itemProfile.selectRect!.height / viewportScale,
                  );
                  final selected = sectionRect.overlaps(itemRect);
                  if (selected) {
                    if (!panTool && !eraseTool && !multiSelect) {
                      context.read<SelectedBloc>().add(
                        const SetHoverSelected(selected: true),
                      );
                    }
                    onSelectedId = id;
                    break;
                  }
                }
              }
              if (onSelectedId != null) {
                if (eraseTool) {
                  controller.selection.erased = true;
                  context.read<StageBloc>().add(
                    RemoveItemEvent(
                      idList: [onSelectedId!],
                      itemBloc: itemBloc,
                      layerBloc: context.read<LayerBloc>(),
                    ),
                  );
                } else {
                  if (!selectedList.contains(onSelectedId) || multiSelect) {
                    context.read<StageBloc>().add(
                      PickItemEvent(
                        idList: [onSelectedId!],
                        itemBloc: itemBloc,
                        multiSelect: multiSelect,
                      ),
                    );
                  }
                }
              } else {
                if (multiSelect) {
                  controller.marqueeStart = details.localPosition;
                  controller.marqueeEnd = details.localPosition;
                } else {
                  context.read<SelectedBloc>().add(const ClearSelectedList());
                }
              }
            }
          },
          onPointerUp: (details) {
            if (enable && multiSelect) {
              controller.marqueeStart = null;
              controller.marqueeEnd = null;
              if (controller.selection.rectangleSelected) {
                final selectedList = <String>[];
                for (final id in idList) {
                  final itemProfile = itemStore[id]!;
                  if (controller.marqueeRect!.overlaps(
                    itemProfile.selectRect!,
                  )) {
                    selectedList.add(id);
                  }
                }
                if (selectedList.isNotEmpty) {
                  if (eraseTool) {
                    controller.selection.erased = true;
                    context.read<StageBloc>().add(
                      RemoveItemEvent(
                        idList: selectedList,
                        itemBloc: itemBloc,
                        layerBloc: context.read<LayerBloc>(),
                      ),
                    );
                  } else {
                    context.read<StageBloc>().add(
                      PickItemEvent(
                        idList: selectedList,
                        itemBloc: itemBloc,
                        multiSelect: multiSelect,
                        skipParallax: true,
                      ),
                    );
                  }
                }
              }
            }
            if (controller.selection.rectangleSelected) {
              controller.selection.rectangleSelected = false;
            }
            if (controller.selection.moved) {
              final actionService = ActionService<ActionModelType>(
                actionType: ActionType.moveItem,
                data: {
                  ActionModelType.mapPieces: stageBloc.clonePieces(),
                  ActionModelType.mapEvents: stageBloc.cloneEvents(),
                },
                change: (data) {
                  final pieces =
                      data![ActionModelType.mapPieces]
                          as HashMap<String, MapPieceItem>;
                  final events =
                      data[ActionModelType.mapEvents]
                          as HashMap<String, MapEventItem>;
                  stageBloc.add(
                    UpdateStageState(
                      stageState: stageBloc.state.copyWith(
                        events: events,
                        pieces: pieces,
                      ),
                    ),
                  );
                  itemBloc.add(const ItemStoreUpdated());
                  historyBloc.add(const UpdateIndexEvent());
                },
              );
              historyBloc.add(CaptureState(state: actionService));
              controller.selection.moved = false;
            }
            if (controller.selection.erased) {
              final actionService = ActionService<ActionModelType>(
                actionType: ActionType.eraseItem,
                data: {
                  ActionModelType.mapPieces: stageBloc.clonePieces(),
                  ActionModelType.mapEvents: stageBloc.cloneEvents(),
                },
                change: (data) {
                  final pieces =
                      data![ActionModelType.mapPieces]
                          as HashMap<String, MapPieceItem>;
                  final events =
                      data[ActionModelType.mapEvents]
                          as HashMap<String, MapEventItem>;
                  stageBloc.add(
                    UpdateStageState(
                      stageState: stageBloc.state.copyWith(
                        events: events,
                        pieces: pieces,
                      ),
                    ),
                  );
                  final layerKeys = <int>[];
                  for (final e in pieces.values) {
                    if (!layerKeys.contains(e.layer)) {
                      layerKeys.add(e.layer);
                    }
                  }
                  for (final layerId in layerKeys) {
                    layerBloc.updateNodeParallax(layerId, pieceItems: pieces);
                  }
                  layerBloc.updateTree(refresh: true);
                  // historyBloc.add(const UpdateIndexEvent());
                },
              );
              historyBloc.add(CaptureState(state: actionService));
              controller.selection.erased = false;
            }
          },
          onPointerCancel: (details) {
            if (enable && multiSelect) {
              controller.marqueeStart = null;
              controller.marqueeEnd = null;
              itemBloc.add(const ItemStoreUpdated());
            }
          },
          onPointerMove: (details) {
            final viewportScale =
                controller.transformationController.value.getMaxScaleOnAxis();
            void onPanUpdate() {
              context.read<StageBloc>().add(
                UpdateItemPosition(
                  x: details.delta.dx / viewportScale,
                  y: details.delta.dy / viewportScale,
                  itemBloc: itemBloc,
                ),
              );
            }

            if (enable) {
              if (multiSelect) {
                if (controller.marqueeStart != null &&
                    controller.marqueeEnd != null) {
                  controller.marqueeEnd = details.localPosition;
                  controller.setMarqueeSelection();
                  controller.selection.rectangleSelected = true;
                } else {
                  onPanUpdate();
                }
                itemBloc.add(const ItemStoreUpdated());
              } else {
                onPanUpdate();
                itemBloc.add(const ItemStoreUpdated());
              }
            }
          },
          child: builder(context, selectedState),
        );
      },
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<Map<String, ItemProfile>>('itemStore', itemStore),
    );
    properties.add(DiagnosticsProperty<bool>('multiSelect', multiSelect));
    properties.add(DiagnosticsProperty<bool>('eraseTool', eraseTool));
    properties.add(DiagnosticsProperty<bool>('panTool', panTool));
    properties.add(DiagnosticsProperty<bool>('resize', resize));
    properties.add(
      ObjectFlagProperty<BlocWidgetBuilder<SelectedState>>.has(
        'builder',
        builder,
      ),
    );
  }
}
