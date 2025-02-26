part of 'resource_bloc.dart';

sealed class ResourceEvent extends Equatable {
  const ResourceEvent();

  @override
  List<Object> get props => [];
}

final class LoadResourceByWorldName extends ResourceEvent {
  const LoadResourceByWorldName({
    required this.events,
    required this.worldName,
    required this.animationDetails,
    required this.itemUpdate,
    this.notifyType = NotifyType.none,
  });

  final HashMap<String, MapEventItem> events;

  final String worldName;

  final Map<String, AnimationDetails> animationDetails;

  final NotifyType notifyType;

  final void Function() itemUpdate;

  @override
  List<Object> get props => [events, worldName, animationDetails, notifyType];
}

final class ResourceLoading extends ResourceEvent {
  const ResourceLoading();

  @override
  List<Object> get props => [];
}

final class ResourceClear extends ResourceEvent {
  const ResourceClear();

  @override
  List<Object> get props => [];
}
