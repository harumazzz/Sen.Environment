import 'dart:collection';

import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../canvas/canvas_bloc.dart';
import '../history/history_bloc.dart';
import '../init_bloc/init_bloc.dart';
import '../item/item_bloc.dart';
import '../layer/layer_bloc.dart';
import '../resource/resource_bloc.dart';
import '../section/section_bloc.dart';
import '../selected/selected_bloc.dart';
import '../setting/setting_bloc.dart';
import '../suggestion/suggestion_bloc.dart';
import '../../include/uuid.dart';
import '../../models/action_service.dart';
import '../../models/event_node.dart';
import '../../models/layer_node.dart';
import '../../models/map_const.dart';
import '../../../../model/worldmap.dart';
import '../../models/action_model.dart';
import '../../../../i18n/app_localizations.dart';

part 'stage_event.dart';
part 'stage_state.dart';

class StageBloc extends Bloc<StageEvent, StageState> {
  StageBloc({
    required this.cubit,
    required this.selectedBloc,
    // required this.layerBloc,
    required this.historyBloc,
    required this.canvasBloc,
    required this.resourceBloc,
    required this.suggestionBloc,
    required this.sectionBloc,
    required this.settingBloc,
    required this.initBloc,
    required this.los,
  }) : super(StageState.initailize()) {
    // on<InitailizeStage>(_initailizeWorld);
    on<LoadWorldEvent>(_loadWorld);
    on<ChangeResouceWorldEvent>(_changeWorldResource);
    on<ClearWorldEvent>(_clearWorld);
    on<UpdateBoundingRect>(_updateBoundingRect);
    on<SetWorldName>(_setWorldName);
    on<SetWorldId>(_setWorldId);
    on<SetResGroupId>(_setResId);
    on<SetWorldResource>(_setWorldResource);
    on<PickItemEvent>(_pickItem);
    on<RemoveItemEvent>(_removeItem);
    on<CloneItemEvent>(_cloneItem);
    on<AddIslandItemEvent>(_addIslandItem);
    on<AddEventItemEvent>(_addEventItem);
    on<UpdateStageState>(_updateState);
    on<UpdateMapInformation>(_updateMapInformation);
    on<UpdateItemPosition>(_updateItemPosition);
  }

  final MapEditorConfigurationCubit cubit;

  final SelectedBloc selectedBloc;

  final HistoryBloc historyBloc;

  final CanvasBloc canvasBloc;

  final ResourceBloc resourceBloc;

  final SectionBloc sectionBloc;

  final SettingBloc settingBloc;

  final SuggestionBloc suggestionBloc;

  final InitBloc initBloc;

  final AppLocalizations los;

  HashMap<String, MapPieceItem> clonePieces({
    HashMap<String, MapPieceItem>? currentPieces,
  }) {
    final newPieces = HashMap<String, MapPieceItem>();
    final oldPieces = currentPieces ?? state.pieces;
    for (final key in oldPieces.keys) {
      newPieces[key] = oldPieces[key]!.clone();
    }
    return newPieces;
  }

  HashMap<String, MapEventItem> cloneEvents({
    HashMap<String, MapEventItem>? currentEvents,
  }) {
    final newEvents = HashMap<String, MapEventItem>();
    final oldEvents = currentEvents ?? state.events;
    for (final key in oldEvents.keys) {
      newEvents[key] = oldEvents[key]!.clone();
    }
    return newEvents;
  }

  void _updateMapInformation(
    UpdateMapInformation event,
    Emitter<StageState> emit,
  ) {
    emit(
      state.copyWith(
        worldName: event.worldName,
        worldId: event.worldId,
        resGroupId: event.resId,
      ),
    );
  }

  void _updateItemPosition(UpdateItemPosition event, Emitter<StageState> emit) {
    final selectedList = selectedBloc.state.selectedList;
    final itemStore = event.itemBloc.state.itemStore;
    for (final id in selectedList) {
      if (!itemStore.containsKey(id)) {
        continue;
      }
      final itemProfile = itemStore[id]!;
      if (itemProfile.isEvent) {
        final itemEvent = state.events[id]!;
        itemEvent.position.x += event.x;
        itemEvent.position.y += event.y;
      } else {
        final piece = state.pieces[id]!;
        piece.position.x += event.x;
        piece.position.y += event.y;
      }
      canvasBloc.state.canvasController.selection.moved = true;
    }
  }

  void _changeWorldResource(
    ChangeResouceWorldEvent event,
    Emitter<StageState> emit,
  ) {
    final worldResource = event.worldResource;
    final config = cubit.state.configModel;
    resourceBloc.add(const ResourceLoading());
    resourceBloc.add(
      LoadResourceByWorldName(
        events: state.events,
        worldName: worldResource,
        animationDetails: config.resource.worldmap[worldResource]!,
        itemUpdate: () => event.itemBloc.add(const ItemStoreUpdated()),
        notifyType: NotifyType.loadResource,
      ),
    );

    final newState = state.copyWith(worldResource: worldResource);
    if (event.layerBloc.isEmptyNode()) {
      event.layerBloc.add(const InitailizeTreeController());
      historyBloc.add(
        InitailizeCapture(
          state: _mapInitializeService(
            actionType: ActionType.loadWorldResource,
            stageState: state,
            stageBloc: event.stageBloc,
            layerBloc: event.layerBloc,
          ),
        ),
      );
    }
    emit(newState);
  }

  Position _getScreenItemPosition() {
    final controllerTransform =
        canvasBloc.state.canvasController.transformationController.value;
    final viewportScale = controllerTransform.getMaxScaleOnAxis();
    final view = WidgetsBinding.instance.platformDispatcher.views.first;
    final screenWidth = view.physicalSize.width;
    final screenHeight = view.physicalSize.height;

    final currentPosX =
        -(controllerTransform[12] - state.boundingRect.x) / viewportScale;
    final currentPosY =
        -(controllerTransform[13] - state.boundingRect.y) / viewportScale;
    // final screenWidth = view.physicalSize.width;
    // final screenHeight = view.physicalSize.height;
    return Position(
      x: currentPosX - screenWidth / 8,
      y: currentPosY - screenHeight / 4,
    );
  }

  void _updateState(UpdateStageState event, Emitter<StageState> emit) {
    if (event.copy) {
      emit(event.stageState.copyWith(worldResource: state.worldResource));
    } else {
      emit(event.stageState);
    }
  }

  void _addIslandItem(AddIslandItemEvent event, Emitter<StageState> emit) {
    final position = _getScreenItemPosition();
    final lastLayerIndex =
        event.layerBloc.state.treeController.roots.first.children.keys.first;
    final piece = MapPieceItem(
      imageID: event.imageId,
      pieceType: event.isAnimation ? PieceType.animation : PieceType.image,
      position: position,
      parallax: 0,
      layer: lastLayerIndex,
      scaleX: 1.0,
      scaleY: 1.0,
      isArtFlipped: false,
      rotationAngle: 0,
      rotationRate: 0.0,
    );
    final id = uuid.v4();
    state.pieces[id] = piece;
    if (!settingBloc.state.muteAudio) {
      cubit.state.editorResource.setItemSound.resume();
    }
    event.layerBloc.updateNodeParallax(lastLayerIndex);
    event.layerBloc.updateTree(refresh: true);
    final stageBloc = event.stageBloc;
    final actionService = ActionService<ActionModelType>(
      actionType: ActionType.addItem,
      data: {
        ActionModelType.mapPieces: clonePieces(),
        ActionModelType.mapEvents: cloneEvents(),
        ActionModelType.int: lastLayerIndex,
      },
      change: (data) {
        final pieces =
            data![ActionModelType.mapPieces] as HashMap<String, MapPieceItem>;
        final events =
            data[ActionModelType.mapEvents] as HashMap<String, MapEventItem>;
        stageBloc.add(
          UpdateStageState(
            stageState: state.copyWith(events: events, pieces: pieces),
          ),
        );
        event.layerBloc.updateNodeParallax(
          data[ActionModelType.int] as int,
          pieceItems: pieces,
        );
        event.layerBloc.updateTree(refresh: true);
      },
    );
    historyBloc.add(CaptureState(state: actionService));
    stageBloc.add(
      PickItemEvent(idList: [id], itemBloc: event.itemBloc, playSound: false),
    );
  }

  void _addEventItem(AddEventItemEvent event, Emitter<StageState> emit) async {
    final position = _getScreenItemPosition();
    final eventId =
        (state.events.isNotEmpty
            ? (state.events.values
                .reduce((a, b) => a.eventID > b.eventID ? a : b)
                .eventID)
            : 0) +
        1;
    final eventItem = MapEventItem(
      eventType: EventType.none,
      eventID: eventId,
      position: position,
      autoVisible: false,
      name: '',
      unlockedFrom: '',
      visibleFrom: '',
      parentEvent: '',
    );
    final configModel = cubit.state.configModel;
    switch (event.eventType) {
      case EventNodeType.normal:
        {
          eventItem.eventType = EventType.level;
          eventItem.levelNodeType = LevelNodeType.normal;
          break;
        }
      case EventNodeType.minigame:
        {
          eventItem.eventType = EventType.level;
          eventItem.levelNodeType = LevelNodeType.minigame;
          break;
        }
      case EventNodeType.miniboss:
        {
          eventItem.eventType = EventType.level;
          eventItem.levelNodeType = LevelNodeType.miniboss;
          break;
        }
      case EventNodeType.nonfinalboss:
        {
          eventItem.eventType = EventType.level;
          eventItem.levelNodeType = LevelNodeType.nonfinalboss;
          break;
        }
      case EventNodeType.boss:
        {
          eventItem.eventType = EventType.level;
          eventItem.levelNodeType = LevelNodeType.boss;
          break;
        }
      case EventNodeType.danger:
        {
          eventItem.eventType = EventType.level;
          eventItem.levelNodeType = LevelNodeType.normal;
          eventItem.dataString = '${state.worldName}_dangerroom';
          break;
        }
      case EventNodeType.pinata:
        {
          eventItem.eventType = EventType.pinata;
          break;
        }
      case EventNodeType.plant:
        {
          eventItem.eventType = EventType.plant;
          final plantType = configModel.resource.plant.keys.firstOrNull;
          eventItem.dataString = plantType;
          if (plantType != null &&
              !cubit.state.gameResource.plant.containsKey(plantType)) {
            final settingPath = cubit.state.settingPath;
            cubit.state.gameResource.plant[plantType] = await cubit
                .loadPlantVisualAnimation(
                  '$settingPath/plant/$plantType',
                  plantType,
                  enableCostume: settingBloc.state.plantCostume,
                );
          }
          break;
        }
      case EventNodeType.giftbox:
        {
          eventItem.eventType = EventType.giftbox;
          break;
        }
      case EventNodeType.upgrade:
        {
          eventItem.eventType = EventType.upgrade;
          eventItem.dataString =
              cubit.state.gameResource.upgrade.keys.firstOrNull;
          break;
        }
      case EventNodeType.stargateLeft:
      case EventNodeType.stargate:
        {
          eventItem.eventType = EventType.star_gate;
          eventItem.cost = 0;
          break;
        }
      case EventNodeType.keyGateLeft:
      case EventNodeType.keygate:
        {
          eventItem.eventType = EventType.key_gate;
          eventItem.cost = 0;
          break;
        }
      case EventNodeType.pathNode:
        {
          eventItem.eventType = EventType.path_node;
          break;
        }
      case EventNodeType.doodad:
        {
          eventItem.eventType = EventType.doodad;
          break;
        }
      default:
        break;
    }
    final id = uuid.v4();
    state.events[id] = eventItem;
    if (!settingBloc.state.muteAudio) {
      await cubit.state.editorResource.setItemSound.resume();
    }
    final stageBloc = event.stageBloc;
    final actionService = ActionService<ActionModelType>(
      actionType: ActionType.addItem,
      data: {
        ActionModelType.mapPieces: clonePieces(),
        ActionModelType.mapEvents: cloneEvents(),
      },
      change: (data) {
        final pieces =
            data![ActionModelType.mapPieces] as HashMap<String, MapPieceItem>;
        final events =
            data[ActionModelType.mapEvents] as HashMap<String, MapEventItem>;
        stageBloc.add(
          UpdateStageState(
            stageState: state.copyWith(events: events, pieces: pieces),
          ),
        );
        event.itemBloc.add(const ItemStoreUpdated());
      },
    );
    historyBloc.add(CaptureState(state: actionService));
    event.itemBloc.add(const ItemStoreUpdated());
    stageBloc.add(
      PickItemEvent(idList: [id], itemBloc: event.itemBloc, playSound: false),
    );
  }

  void _pickItem(PickItemEvent event, Emitter<StageState> emit) {
    final idList = event.idList;
    final itemStore = event.itemBloc.state.itemStore;
    if (idList.isNotEmpty) {
      for (final id in idList) {
        final itemProfile = itemStore[id];
        if (itemProfile != null) {
          if (itemProfile.isEvent) {
            sectionBloc.add(const SetSection(section: SectionType.event));
          } else {
            final piece = state.pieces[id]!;
            if (event.skipParallax && piece.parallax != 0) {
              continue;
            }
            if (piece.pieceType == PieceType.animation) {
              sectionBloc.add(const SetSection(section: SectionType.animation));
            } else if (piece.pieceType == PieceType.image) {
              sectionBloc.add(const SetSection(section: SectionType.image));
            }
          }
          if (event.multiSelect) {
            selectedBloc.add(AddSelectionEvent(id: id, toggle: true));
          } else {
            selectedBloc.add(SetSelectionEvent(id: id));
          }
        }
      }
    }
    if (!settingBloc.state.muteAudio && event.playSound) {
      cubit.state.editorResource.pickItemSound.resume();
    }
    event.itemBloc.add(const ItemStoreUpdated());
    /*
    historyBloc.add(CaptureState(
        state: ActionService<ActionModelType>(
            actionType: ActionType.addItem,
            data: {
              ActionModelType.mapPieces: clonePieces(),
              ActionModelType.stageBloc: stateBloc,
              ActionModelType.itemBloc: event.itemBloc
            },
            change: (data) async {
              stateBloc.add(UpdateStageState(
                stageState: (data![ActionModelType.stage] as StageState),
              ));
              itemBloc.add(const ItemStoreUpdated());
            })));
            */
  }

  void _removeItem(RemoveItemEvent event, Emitter<StageState> emit) {
    final idList = event.idList;
    final itemStore = event.itemBloc.state.itemStore;
    if (idList.isNotEmpty) {
      for (final id in idList) {
        final itemProfile = itemStore[id];
        if (itemProfile != null) {
          if (selectedBloc.state.selectedList.contains(id)) {
            selectedBloc.add(
              SelectedListUpdated(
                selectedList:
                    selectedBloc.state.selectedList.toList()..remove(id),
              ),
            );
          }
          if (itemProfile.isEvent) {
            state.events.remove(id);
          } else {
            final pieces = state.pieces;
            final piece = pieces[id];
            if (piece != null && !(event.skipParallax && piece.parallax != 0)) {
              final layerIndex = pieces[id]!.layer;
              pieces.remove(id);
              event.layerBloc.updateNodeParallax(layerIndex);
            }
          }
        }
      }
      event.layerBloc.updateTree(refresh: true);
      if (!settingBloc.state.muteAudio) {
        cubit.state.editorResource.removeItemSound.resume();
      }
    }
  }

  void _cloneItem(CloneItemEvent event, Emitter<StageState> emit) {
    final itemBloc = event.itemBloc;
    final posX = event.positionXAdditional;
    final posY = event.positionYAdditional;
    final newIdList = <String>[];
    for (final id in event.idList) {
      final itemProfile = itemBloc.state.itemStore[id];
      if (itemProfile != null) {
        final newId = uuid.v4();
        if (itemProfile.isEvent) {
          final newEvent = state.events[id]!.clone();
          newEvent.position.x += posX;
          newEvent.position.y += posY;
          state.events[newId] = newEvent;
        } else {
          final newPiece = state.pieces[id]!.clone();
          newPiece.position.x += posX;
          newPiece.position.y += posY;
          state.pieces[newId] = newPiece;
          event.layerBloc.updateNodeParallax(newPiece.layer);
        }
        newIdList.add(newId);
      }
    }
    event.layerBloc.updateTree(refresh: true);
    if (!settingBloc.state.muteAudio) {
      cubit.state.editorResource.setItemSound.resume();
    }
    selectedBloc.add(SelectedListUpdated(selectedList: newIdList));
  }

  void _updateBoundingRect(UpdateBoundingRect event, Emitter<StageState> emit) {
    emit(state.copyWith(boundingRect: event.boundingRect));
  }

  void _setWorldName(SetWorldName event, Emitter<StageState> emit) {
    emit(state.copyWith(worldName: event.worldName));
  }

  void _setWorldId(SetWorldId event, Emitter<StageState> emit) {
    emit(state.copyWith(worldId: event.worldId));
  }

  void _setResId(SetResGroupId event, Emitter<StageState> emit) {
    emit(state.copyWith(resGroupId: event.resId));
  }

  void _setWorldResource(SetWorldResource event, Emitter<StageState> emit) {
    emit(state.copyWith(worldResource: event.worldResource));
  }

  void _resortPiecesLayer(HashMap<String, MapPieceItem> pieces) {
    final treeLayer = SplayTreeMap<int, int>((a, b) => b.compareTo(a));
    for (final piece in pieces.values) {
      if (!treeLayer.containsKey(piece.layer)) {
        treeLayer[piece.layer] = 0;
      }
    }
    var i = treeLayer.keys.length - 1;
    for (final index in treeLayer.keys) {
      treeLayer[index] = i;
      --i;
    }
    for (final piece in pieces.values) {
      piece.layer = treeLayer[piece.layer]!;
    }
  }

  ActionService<ActionModelType> _mapInitializeService({
    required ActionType actionType,
    required StageState stageState,
    required StageBloc stageBloc,
    required LayerBloc layerBloc,
  }) {
    return ActionService<ActionModelType>(
      actionType: actionType,
      data: {ActionModelType.stage: stageState},
      change: (data) {
        final state = data![ActionModelType.stage] as StageState;
        stageBloc.add(UpdateStageState(stageState: state));
        layerBloc.add(const InitailizeTreeController());
        layerBloc.add(const InitailizeLayerNode());
      },
    );
  }

  Iterable<String> getLevelName() {
    return state.events.values.map((e) => e.name);
  }

  HashMap<(int, int), ItemProperty?> _createPieceProperty(
    HashMap<String, MapPieceItem> pieces,
  ) {
    final property = HashMap<(int, int), ItemProperty>();
    for (final e in pieces.values) {
      final id = (e.layer, e.parallax);
      if (!property.containsKey(id)) {
        property[id] = ItemProperty();
      }
    }
    return property;
  }

  Future<void> _loadWorld(
    LoadWorldEvent event,
    Emitter<StageState> emit,
  ) async {
    final worldData = event.worldData;
    final config = cubit.state.configModel;
    var worldResource = 'none';
    final pieces = HashMap<String, MapPieceItem>();
    final events = HashMap<String, MapEventItem>();
    for (final piece in worldData.pieces) {
      pieces[uuid.v4()] = piece;
    }
    for (final event in worldData.events) {
      events[uuid.v4()] = event;
    }
    _resortPiecesLayer(pieces);
    if (config.resource.worldmap.containsKey(worldData.worldName)) {
      worldResource = worldData.worldName;
      resourceBloc.add(const ResourceLoading());
      resourceBloc.add(
        LoadResourceByWorldName(
          events: events,
          worldName: worldResource,
          animationDetails: config.resource.worldmap[worldResource]!,
          notifyType: NotifyType.loadWorld,
          itemUpdate: () => event.itemBloc.add(const ItemStoreUpdated()),
        ),
      );
    } else {
      //resourceBloc.add(const ResourceLoading());
      resourceBloc.add(const ResourceClear());
    }
    selectedBloc.add(const ClearSelectedList());
    event.layerBloc.add(const InitailizeTreeController());
    event.layerBloc.add(const InitailizeLayerNode());
    canvasBloc.add(const InitCameraViewOffset());
    event.itemBloc.add(const ItemStoreUpdated());
    suggestionBloc.add(UpdateEventNameList(eventNameList: getLevelName()));
    final newState = StageState(
      worldResource: worldResource,
      worldName: worldData.worldName,
      worldId: worldData.worldID,
      resGroupId: worldData.resGroupID,
      boundingRect: worldData.boundingRect.clone(),
      pieces: pieces,
      events: events,
      pieceProperty: _createPieceProperty(pieces),
      eventProperty: ItemProperty(),
    );

    historyBloc.add(
      InitailizeCapture(
        state: _mapInitializeService(
          actionType: ActionType.openWorldMap,
          stageState: newState.copyWith(),
          stageBloc: event.stageBloc,
          layerBloc: event.layerBloc,
        ),
      ),
    );
    emit(newState);
  }

  void _clearWorld(ClearWorldEvent event, Emitter<StageState> emit) {
    final emptyState = StageState.initailize();
    sectionBloc.add(const SetSection(section: SectionType.select));
    selectedBloc.add(const ClearSelectedList());
    emit(emptyState);
    canvasBloc.add(const InitCameraViewOffset());
    resourceBloc.add(const ResourceClear());
    event.layerBloc.add(const ClearLayerNode());
    historyBloc.add(const ClearHistoryEvent());
    event.itemUpdate();
    if (!settingBloc.state.muteAudio) {
      cubit.state.editorResource.clearMapSound.resume();
    }
    initBloc.add(ShowSnackBarEvent(text: los.worldmap_has_been_cleared));
  }

  // void _initailizeWorld(InitailizeStage event, Emitter<StageState> emit) {}
}
