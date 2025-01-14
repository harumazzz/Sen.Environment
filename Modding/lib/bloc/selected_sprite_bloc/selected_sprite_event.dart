part of 'selected_sprite_bloc.dart';

@immutable
sealed class SelectedSpriteEvent extends Equatable {
  const SelectedSpriteEvent();

  @override
  List<Object?> get props => [];
}

final class SelectedSpriteResetEvent extends SelectedSpriteEvent {
  const SelectedSpriteResetEvent();
}

final class SelectedSpriteToggleEvent extends SelectedSpriteEvent {
  final int index;

  const SelectedSpriteToggleEvent({
    required this.index,
  });

  @override
  List<Object?> get props => [index];
}

final class SelectedSpriteAllocateEvent extends SelectedSpriteEvent {
  final int size;

  const SelectedSpriteAllocateEvent({
    required this.size,
  });

  @override
  List<Object?> get props => [size];
}

final class SelectedSpriteEnableAllEvent extends SelectedSpriteEvent {
  const SelectedSpriteEnableAllEvent();
}

final class SelectedSpriteDisableAllEvent extends SelectedSpriteEvent {
  const SelectedSpriteDisableAllEvent();
}
