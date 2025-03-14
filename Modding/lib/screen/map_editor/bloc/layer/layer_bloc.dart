import 'dart:collection';

import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:flutter_fancy_tree_view/flutter_fancy_tree_view.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../../../model/worldmap.dart';
import '../history/history_bloc.dart';
import '../item/item_bloc.dart';
import '../selected/selected_bloc.dart';
import '../stage/stage_bloc.dart';
import '../../models/action_model.dart';
import '../../models/action_service.dart';
import '../../models/layer_node.dart';
import '../../../../i18n/app_localizations.dart';

part 'layer_event.dart';
part 'layer_state.dart';

class LayerBloc extends Bloc<LayerEvent, LayerState> {
  LayerBloc({
    required this.historyBloc,
    required this.stageBloc,
    required this.selectedBloc,
    required this.itemBloc,
    required this.los,
  }) : super(LayerState.initailize()) {
    on<InitailizeTreeController>(_initializeController);
    on<InitailizeLayerNode>(_initailizeLayerNode);
    on<ClearLayerNode>(_clearLayerNode);
  }

  final HistoryBloc historyBloc;

  final StageBloc stageBloc;

  final SelectedBloc selectedBloc;

  final ItemBloc itemBloc;

  final AppLocalizations los;

  void _clearLayerNode(ClearLayerNode event, Emitter<LayerState> emit) {
    emit(
      LayerState(
        treeController: TreeController(
          roots: [],
          childrenProvider: (LayerNode node) => node.children.values,
        ),
      ),
    );
  }

  void updateTree({required bool refresh}) {
    state.treeController.rebuild();
    if (refresh) {
      itemBloc.add(const ItemStoreUpdated());
    }
  }

  void _itemStatusChange(ItemProperty entry, LockProperties status) {
    entry.status = status;
  }

  ItemProperty _entryPieceProperty((int, int) id) {
    if (!stageBloc.state.pieceProperty.containsKey(id)) {
      return stageBloc.state.pieceProperty[id] = ItemProperty();
    }
    return stageBloc.state.pieceProperty[id]!;
  }

  void _actionListItemStatusResest(
    List<(IconData, dynamic Function())> actionList,
    (int, int)? id,
  ) {
    actionList.clear();
    actionList.addAll([
      (
        Symbols.lock,
        () {
          final itemProperty =
              id != null
                  ? _entryPieceProperty(id)
                  : stageBloc.state.eventProperty;
          _itemStatusChange(itemProperty, LockProperties.lockVisible);
          updateTree(refresh: true);
        },
      ),
      (
        Symbols.lock_open,
        () {
          final itemProperty =
              id != null
                  ? _entryPieceProperty(id)
                  : stageBloc.state.eventProperty;
          _itemStatusChange(itemProperty, LockProperties.visible);
          updateTree(refresh: true);
        },
      ),
      (
        Symbols.visibility,
        () {
          final itemProperty =
              id != null
                  ? _entryPieceProperty(id)
                  : stageBloc.state.eventProperty;
          _itemStatusChange(itemProperty, LockProperties.opacityVisible);
          updateTree(refresh: true);
        },
      ),
      (
        Symbols.visibility_lock,
        () {
          final itemProperty =
              id != null
                  ? _entryPieceProperty(id)
                  : stageBloc.state.eventProperty;
          _itemStatusChange(itemProperty, LockProperties.invisible);
          updateTree(refresh: true);
        },
      ),
      (
        Symbols.visibility_off,
        () {
          final itemProperty =
              id != null
                  ? _entryPieceProperty(id)
                  : stageBloc.state.eventProperty;
          _itemStatusChange(itemProperty, LockProperties.visible);
          updateTree(refresh: true);
        },
      ),
    ]);
  }

  bool _visibleOnAccepted(
    ItemProperty entry,
    List<LockProperties> statusAcceptedList,
  ) {
    return statusAcceptedList.contains(entry.status);
  }

  void _actionListItemStatusAcceptedReset(
    List<bool Function()> onActionAccepted,
    (int, int)? id,
  ) {
    onActionAccepted.clear();
    onActionAccepted.addAll([
      () {
        final itemProperty =
            id != null
                ? _entryPieceProperty(id)
                : stageBloc.state.eventProperty;
        return _visibleOnAccepted(itemProperty, [LockProperties.visible]);
      },
      () {
        final itemProperty =
            id != null
                ? _entryPieceProperty(id)
                : stageBloc.state.eventProperty;
        return _visibleOnAccepted(itemProperty, [LockProperties.lockVisible]);
      },
      () {
        final itemProperty =
            id != null
                ? _entryPieceProperty(id)
                : stageBloc.state.eventProperty;
        return _visibleOnAccepted(itemProperty, [LockProperties.visible]);
      },
      () {
        final itemProperty =
            id != null
                ? _entryPieceProperty(id)
                : stageBloc.state.eventProperty;
        return _visibleOnAccepted(itemProperty, [
          LockProperties.opacityVisible,
        ]);
      },
      () {
        final itemProperty =
            id != null
                ? _entryPieceProperty(id)
                : stageBloc.state.eventProperty;
        return _visibleOnAccepted(itemProperty, [LockProperties.invisible]);
      },
    ]);
  }

  void _layerActionReset(
    List<(IconData, dynamic Function())> actionList,
    int index,
  ) {
    actionList.clear();
    actionList.addAll([
      (
        Symbols.delete_sweep,
        () {
          _deleteLayer(index);
          updateTree(refresh: true);
        },
      ),
      (
        Symbols.text_select_move_down,
        () {
          _mergeDownLayer(index);
          updateTree(refresh: true);
        },
      ),
      (
        Symbols.move_selection_up,
        () {
          _moveUpLayer(index);
          updateTree(refresh: true);
        },
      ),
      (
        Symbols.move_selection_down,
        () {
          _moveDownLayer(index);
          updateTree(refresh: true);
        },
      ),
    ]);
  }

  void _layerActionAcceptedReset(
    List<bool Function()> onActionAccepted,
    int index,
  ) {
    onActionAccepted.clear();
    onActionAccepted.addAll([
      _moreLayers,
      () {
        return _moveDownLayerOnAccepted(index);
      },
      () {
        return _moveUpLayerOnAccepted(index);
      },
      () {
        return _moveDownLayerOnAccepted(index);
      },
    ]);
  }

  void _deleteLayer(int target) {
    final node = _getPiecesLayer();
    final pieces = stageBloc.state.pieces;
    final layerKeys = pieces.keys.toList();
    selectedBloc.add(const ClearSelectedList());
    for (final key in layerKeys) {
      final piece = pieces[key]!;
      if (piece.layer == target) {
        pieces.remove(key);
      }
    }
    node.children.remove(target);
    final actionService = ActionService<ActionModelType>(
      actionType: ActionType.deleteLayer,
      data: {
        ActionModelType.mapPieces: stageBloc.clonePieces(),
        ActionModelType.mapEvents: stageBloc.cloneEvents(),
        ActionModelType.layerNode: node.copyWith(),
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

        final layerNode =
            (data[ActionModelType.layerNode] as LayerNode).copyWith();
        state.treeController.roots.first.children.clear();
        state.treeController.roots.first.children.addAll(layerNode.children);
        final layerKeys = <int>[];
        for (final e in pieces.values) {
          if (!layerKeys.contains(e.layer)) {
            layerKeys.add(e.layer);
          }
        }
        for (final layerId in layerKeys) {
          updateNodeParallax(layerId, pieceItems: pieces);
        }
        updateTree(refresh: true);
        // historyBloc.add(const UpdateIndexEvent());
      },
    );
    historyBloc.add(CaptureState(state: actionService));
  }

  void _mergeDownLayer(int target) {
    final node = _getPiecesLayer();
    final layerList = node.children.keys.toList();
    final targetIndex = layerList.indexOf(target);
    final entryIndex = layerList[targetIndex + 1];
    final pieces = stageBloc.state.pieces;
    final entry = node.children[entryIndex]!;
    for (final key in pieces.keys) {
      final piece = pieces[key]!;
      if (piece.layer == target) {
        piece.layer = entryIndex;
      }
    }
    _layerActionReset(entry.actionList, entryIndex);
    _layerActionAcceptedReset(entry.onActionAccepted, entryIndex);
    updateNodeParallax(entryIndex);
    node.children.remove(target);
    final actionService = ActionService<ActionModelType>(
      actionType: ActionType.mergeDownLayer,
      data: {
        ActionModelType.mapPieces: stageBloc.clonePieces(),
        ActionModelType.mapEvents: stageBloc.cloneEvents(),
        ActionModelType.layerNode: node.copyWith(),
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

        final layerNode =
            (data[ActionModelType.layerNode] as LayerNode).copyWith();
        state.treeController.roots.first.children.clear();
        state.treeController.roots.first.children.addAll(layerNode.children);
        final layerKeys = <int>[];
        for (final e in pieces.values) {
          if (!layerKeys.contains(e.layer)) {
            layerKeys.add(e.layer);
          }
        }
        for (final layerId in layerKeys) {
          updateNodeParallax(layerId, pieceItems: pieces);
        }
        updateTree(refresh: true);
        // historyBloc.add(const UpdateIndexEvent());
      },
    );
    historyBloc.add(CaptureState(state: actionService));
  }

  void _moveLayer(int entryIndex, int targetIndex, {bool moveUp = true}) {
    final node = _getPiecesLayer();
    final entry = node.children[entryIndex]!;
    final target = node.children[targetIndex]!;
    _layerActionReset(entry.actionList, targetIndex);
    _layerActionAcceptedReset(entry.onActionAccepted, targetIndex);
    _layerActionReset(target.actionList, entryIndex);
    _layerActionAcceptedReset(target.onActionAccepted, entryIndex);
    node.children[targetIndex] = entry;
    node.children[entryIndex] = target;
    final pieces = stageBloc.state.pieces;
    final layerKeys = pieces.keys.toList();
    for (final key in layerKeys) {
      final piece = pieces[key]!;
      if (piece.layer == entryIndex) {
        piece.layer = targetIndex;
      } else if (piece.layer == targetIndex) {
        piece.layer = entryIndex;
      }
    }
    final actionService = ActionService<ActionModelType>(
      actionType: moveUp ? ActionType.moveUpLayer : ActionType.moveDownLayer,
      data: {
        ActionModelType.mapPieces: stageBloc.clonePieces(),
        ActionModelType.mapEvents: stageBloc.cloneEvents(),
        ActionModelType.layerNode: node.copyWith(),
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

        final layerNode =
            (data[ActionModelType.layerNode] as LayerNode).copyWith();
        state.treeController.roots.first.children.clear();
        state.treeController.roots.first.children.addAll(layerNode.children);
        final layerKeys = <int>[];
        for (final e in pieces.values) {
          if (!layerKeys.contains(e.layer)) {
            layerKeys.add(e.layer);
          }
        }
        for (final layerId in layerKeys) {
          updateNodeParallax(layerId, pieceItems: pieces);
        }
        updateTree(refresh: true);
        // historyBloc.add(const UpdateIndexEvent());
      },
    );
    historyBloc.add(CaptureState(state: actionService));
  }

  void _moveUpLayer(int target) {
    final layerList = _getPiecesLayer().children.keys.toList();
    final targetIndex = layerList.indexOf(target);
    _moveLayer(target, layerList[targetIndex - 1]);
  }

  void _moveDownLayer(int target) {
    final layerList = _getPiecesLayer().children.keys.toList();
    final targetIndex = layerList.indexOf(target);
    _moveLayer(target, layerList[targetIndex + 1], moveUp: false);
  }

  bool _moreLayers() {
    //return layerNameStore.length > 1;
    return _getPiecesLayer().children.length > 1;
  }

  bool _moveUpLayerOnAccepted(int target) {
    // return layerNameStore.keys.last != target && moreLayers();
    return target < _getPiecesLayer().children.length - 1 && _moreLayers();
  }

  bool _moveDownLayerOnAccepted(int target) {
    // return layerNameStore.keys.first != target && moreLayers();
    return target > 0 && _moreLayers();
  }

  void updateNodeParallax(
    int target, {
    HashMap<String, MapPieceItem>? pieceItems,
  }) {
    final node = _getPiecesLayer().children[target]!.children;
    final pieces = pieceItems ?? stageBloc.state.pieces;
    final parallaxCount = <int, int>{};
    for (final piece in pieces.values) {
      if (piece.layer == target) {
        parallaxCount[piece.parallax] =
            (parallaxCount[piece.parallax] ?? 0) + 1;
      }
    }
    for (final parallax in parallaxCount.keys) {
      if (!node.containsKey(parallax)) {
        final child = LayerNode(
          title: '${los.parallax} $parallax (${parallaxCount[parallax]})',
          icon: Symbols.landscape,
          type: NodeType.item,
        );
        final id = (target, parallax);
        _actionListItemStatusResest(child.actionList, id);
        _actionListItemStatusAcceptedReset(child.onActionAccepted, id);
        node[parallax] = child;
      } else {
        node[parallax]!.title =
            '${los.parallax} $parallax (${parallaxCount[parallax]})';
      }
    }
    node.removeWhere((e, value) => !parallaxCount.containsKey(e));
  }

  void _createNewLayer({
    String name = 'New Layer',
    bool captureHistory = false,
  }) {
    final node = _getPiecesLayer();
    final layerNameList = node.children.values.map((e) => e.title).toList();
    final layerName = name;
    var newLayerName = layerName;
    var index = 2;
    while (layerNameList.contains(newLayerName)) {
      newLayerName = '$layerName $index';
      ++index;
    }
    final nextIndex =
        node.children.isNotEmpty ? node.children.keys.first + 1 : 0;
    final child = LayerNode(
      title: newLayerName,
      icon: Symbols.folder,
      iconExpanded: Symbols.folder_open,
      type: NodeType.parent,
      children: SplayTreeMap(),
    );
    _layerActionReset(child.actionList, nextIndex);
    _layerActionAcceptedReset(child.onActionAccepted, nextIndex);
    node.children[nextIndex] = child;
    if (captureHistory) {
      final actionService = ActionService<ActionModelType>(
        actionType: ActionType.createNewLayer,
        data: {ActionModelType.layerNode: node.copyWith()},
        change: (data) {
          final layerNode =
              (data![ActionModelType.layerNode] as LayerNode).copyWith();
          state.treeController.roots.first.children.clear();
          state.treeController.roots.first.children.addAll(layerNode.children);
          updateTree(refresh: false);
        },
      );
      historyBloc.add(CaptureState(state: actionService));
    }
  }

  void _initailizeLayerNode(
    InitailizeLayerNode event,
    Emitter<LayerState> emit,
  ) {
    final node = _getPiecesLayer().children;
    final pieces = stageBloc.state.pieces;
    final largestLayer =
        pieces.values.isNotEmpty
            ? pieces.values.reduce((a, b) => a.layer > b.layer ? a : b).layer
            : 0;
    while (node.length <= largestLayer) {
      _createNewLayer(name: 'Layer ${node.length}');
      updateNodeParallax(node.length - 1);
    }
  }

  void _initializeController(
    InitailizeTreeController event,
    Emitter<LayerState> emit,
  ) {
    state.treeController.roots = [
      LayerNode(
        title: los.piece_layer,
        icon: Symbols.folder,
        iconExpanded: Symbols.folder_open,
        type: NodeType.root,
        actionList: [
          (
            Symbols.create_new_folder,
            () {
              _createNewLayer(captureHistory: true);
              updateTree(refresh: true);
            },
          ),
        ],
        onActionAccepted: [() => true],
      ),
      LayerNode(
        title: los.event_settings,
        icon: Symbols.kid_star,
        type: NodeType.item,
      ),
    ];
    _actionListItemStatusResest(
      state.treeController.roots.last.actionList,
      null,
    );
    _actionListItemStatusAcceptedReset(
      state.treeController.roots.last.onActionAccepted,
      null,
    );
    _createNewLayer(name: 'Default');
    emit(state);
  }

  /*
  LayerNode _getEventsSetting() {
    return state.treeController.roots.last;
  }
  */

  LayerNode _getPiecesLayer() {
    return state.treeController.roots.first;
  }
  /*
  ItemProperty entryProperty(int layer, int parallax) {
    final node = _getPiecesLayer();
    return node.children[layer]?.children[parallax]?.property ?? ItemProperty();
  }
  */

  bool isVaildLayerName(String name) {
    if (name == '') {
      return false;
    }
    final node = _getPiecesLayer();
    for (final layerNode in node.children.values) {
      if (layerNode.title == name) {
        return false;
      }
    }
    return true;
  }

  bool isEmptyNode() {
    return state.treeController.roots.isEmpty;
  }
}
