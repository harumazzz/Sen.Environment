import 'dart:collection';

import 'package:equatable/equatable.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../../../../model/worldmap.dart';
import '../history/history_bloc.dart';
import '../item/item_bloc.dart';
import '../layer/layer_bloc.dart';
import '../selected/selected_bloc.dart';
import '../setting/setting_bloc.dart';
import '../stage/stage_bloc.dart';
import '../../include/controller.dart';
import '../../models/action_model.dart';
import '../../models/action_service.dart';

part 'shortcut_event.dart';
part 'shortcut_state.dart';

class ShortcutBloc extends Bloc<ShortcutEvent, ShortcutState> {
  ShortcutBloc({
    required this.stageBloc,
    required this.layerBloc,
    required this.selectedBloc,
    required this.itemBloc,
    required this.historyBloc,
    required this.settingBloc,
    required this.cubit,
  }) : super(const ShortcutState());

  final StageBloc stageBloc;

  final LayerBloc layerBloc;

  final SelectedBloc selectedBloc;

  final ItemBloc itemBloc;

  final HistoryBloc historyBloc;

  final SettingBloc settingBloc;

  final MapEditorConfigurationCubit cubit;

  void delete() {
    final selectedList = selectedBloc.state.selectedList;
    stageBloc.add(
      RemoveItemEvent(
        idList: selectedList,
        itemBloc: itemBloc,
        layerBloc: layerBloc,
      ),
    );
    final actionService = ActionService<ActionModelType>(
      actionType: ActionType.eraseItem,
      data: {
        ActionModelType.mapPieces: stageBloc.clonePieces(),
        ActionModelType.mapEvents: stageBloc.cloneEvents(),
      },
      change: (data) {
        final pieces =
            data![ActionModelType.mapPieces] as HashMap<String, MapPieceItem>;
        final events =
            data[ActionModelType.mapEvents] as HashMap<String, MapEventItem>;
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
      },
    );
    historyBloc.add(CaptureState(state: actionService));
  }

  void copy(CanvasController controller) {
    controller.catchPointerHover();
    selectedBloc.add(const CopySelectedList(isCut: false));
  }

  void cut(CanvasController controller) {
    controller.catchPointerHover();
    selectedBloc.add(const CopySelectedList(isCut: true));
  }

  void paste(CanvasController controller) {
    final copyList = selectedBloc.state.copyList;
    final details = controller.getDetailsMove();
    if (copyList.isNotEmpty) {
      if (copyList.contains('paste_cut')) {
        for (final id in copyList) {
          final itemProfile = itemBloc.state.itemStore[id];
          if (itemProfile != null) {
            if (itemProfile.isEvent) {
              final event = stageBloc.state.events[id]!;
              event.position.x += details.dx;
              event.position.y += details.dy;
            } else {
              final piece = stageBloc.state.pieces[id]!;
              piece.position.x += details.dx;
              piece.position.y += details.dy;
            }
          }
        }
        if (!settingBloc.state.muteAudio) {
          cubit.state.editorResource.setItemSound.resume();
        }
        selectedBloc.add(const ClearCopyList());
        itemBloc.add(const ItemStoreUpdated());
      } else {
        stageBloc.add(
          CloneItemEvent(
            idList: copyList,
            itemBloc: itemBloc,
            layerBloc: layerBloc,
            positionXAdditional: details.dx,
            positionYAdditional: details.dy,
          ),
        );
      }
      final actionService = ActionService<ActionModelType>(
        actionType: ActionType.pasteItem,
        data: {
          ActionModelType.mapPieces: stageBloc.clonePieces(),
          ActionModelType.mapEvents: stageBloc.cloneEvents(),
        },
        change: (data) {
          final pieces =
              data![ActionModelType.mapPieces] as HashMap<String, MapPieceItem>;
          final events =
              data[ActionModelType.mapEvents] as HashMap<String, MapEventItem>;
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
    }
  }
}
