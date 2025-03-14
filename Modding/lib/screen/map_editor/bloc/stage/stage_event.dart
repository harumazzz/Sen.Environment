part of 'stage_bloc.dart';

sealed class StageEvent extends Equatable {
  const StageEvent();

  @override
  List<Object> get props => [];
}

final class InitailizeStage extends StageEvent {
  const InitailizeStage();

  @override
  List<Object> get props => [];
}

final class UpdateBoundingRect extends StageEvent {
  const UpdateBoundingRect({required this.boundingRect});

  final BoundingRect boundingRect;
}

final class LoadWorldEvent extends StageEvent {
  const LoadWorldEvent({
    required this.worldData,
    required this.itemBloc,
    required this.layerBloc,
    required this.stageBloc,
  });

  final WorldData worldData;

  final ItemBloc itemBloc;

  final LayerBloc layerBloc;

  final StageBloc stageBloc;

  @override
  List<Object> get props => [worldData, itemBloc, layerBloc, stageBloc];
}

final class ChangeResouceWorldEvent extends StageEvent {
  const ChangeResouceWorldEvent({
    required this.worldResource,
    required this.itemBloc,
    required this.layerBloc,
    required this.stageBloc,
  });

  final String worldResource;

  final ItemBloc itemBloc;

  final LayerBloc layerBloc;

  final StageBloc stageBloc;

  @override
  List<Object> get props => [worldResource, itemBloc, layerBloc, stageBloc];
}

final class UpdateStageState extends StageEvent {
  const UpdateStageState({required this.stageState, this.copy = true});

  final StageState stageState;

  final bool copy;

  @override
  List<Object> get props => [copy, stageState];
}

final class ClearWorldEvent extends StageEvent {
  const ClearWorldEvent({required this.itemUpdate, required this.layerBloc});

  final void Function() itemUpdate;

  final LayerBloc layerBloc;

  @override
  List<Object> get props => [itemUpdate, layerBloc];
}

final class SetWorldName extends StageEvent {
  const SetWorldName({required this.worldName});

  final String worldName;

  @override
  List<Object> get props => [worldName];
}

final class SetWorldId extends StageEvent {
  const SetWorldId({required this.worldId});

  final int worldId;

  @override
  List<Object> get props => [worldId];
}

final class SetResGroupId extends StageEvent {
  const SetResGroupId({required this.resId});

  final int resId;

  @override
  List<Object> get props => [resId];
}

final class SetWorldResource extends StageEvent {
  const SetWorldResource({required this.worldResource});

  final String worldResource;

  @override
  List<Object> get props => [worldResource];
}

final class PickItemEvent extends StageEvent {
  const PickItemEvent({
    required this.idList,
    required this.itemBloc,
    this.playSound = true,
    this.multiSelect = false,
    this.skipParallax = false,
  });

  final Iterable<String> idList;
  final bool multiSelect, playSound, skipParallax;

  final ItemBloc itemBloc;

  @override
  List<Object> get props => [
    idList,
    playSound,
    itemBloc,
    multiSelect,
    skipParallax,
  ];
}

final class RemoveItemEvent extends StageEvent {
  const RemoveItemEvent({
    required this.idList,
    required this.itemBloc,
    required this.layerBloc,
    this.skipParallax = false,
  });

  final Iterable<String> idList;
  final bool skipParallax;
  final LayerBloc layerBloc;

  final ItemBloc itemBloc;

  @override
  List<Object> get props => [idList, skipParallax, itemBloc, layerBloc];
}

final class CloneItemEvent extends StageEvent {
  const CloneItemEvent({
    required this.idList,
    required this.itemBloc,
    required this.layerBloc,
    this.positionXAdditional = 40,
    this.positionYAdditional = 40,
  });

  final Iterable<String> idList;
  final ItemBloc itemBloc;
  final LayerBloc layerBloc;

  final double positionXAdditional, positionYAdditional;

  @override
  List<Object> get props => [
    idList,
    itemBloc,
    positionXAdditional,
    positionYAdditional,
  ];
}

final class AddIslandItemEvent extends StageEvent {
  const AddIslandItemEvent({
    required this.imageId,
    required this.isAnimation,
    required this.layerBloc,
    required this.itemBloc,
    required this.stageBloc,
  });

  final int imageId;
  final bool isAnimation;

  final LayerBloc layerBloc;
  final ItemBloc itemBloc;
  final StageBloc stageBloc;

  @override
  List<Object> get props => [
    imageId,
    isAnimation,
    layerBloc,
    itemBloc,
    stageBloc,
  ];
}

final class AddEventItemEvent extends StageEvent {
  const AddEventItemEvent({
    required this.eventType,
    required this.itemBloc,
    required this.stageBloc,
  });

  final EventNodeType eventType;

  final ItemBloc itemBloc;
  final StageBloc stageBloc;

  @override
  List<Object> get props => [eventType, itemBloc, stageBloc];
}

final class UpdateMapInformation extends StageEvent {
  const UpdateMapInformation({this.worldName, this.worldId, this.resId});

  final String? worldName;

  final int? worldId, resId;

  @override
  List<Object> get props => [];
}

final class UpdateItemPosition extends StageEvent {
  const UpdateItemPosition({
    required this.x,
    required this.y,
    required this.itemBloc,
  });

  final double x;

  final double y;

  final ItemBloc itemBloc;

  @override
  List<Object> get props => [x, y, itemBloc];
}
