part of 'selected_image_bloc.dart';

@immutable
sealed class SelectedImageEvent extends Equatable {
  const SelectedImageEvent();

  @override
  List<Object?> get props => [];
}

final class SelectedImageResetEvent extends SelectedImageEvent {
  const SelectedImageResetEvent();
}

final class SelectedImageToggleEvent extends SelectedImageEvent {
  final int index;

  const SelectedImageToggleEvent({
    required this.index,
  });

  @override
  List<Object?> get props => [index];
}

final class SelectedImageAllocateEvent extends SelectedImageEvent {
  final int size;

  const SelectedImageAllocateEvent({
    required this.size,
  });

  @override
  List<Object?> get props => [size];
}

final class SelectedImageEnableAllEvent extends SelectedImageEvent {
  const SelectedImageEnableAllEvent();
}

final class SelectedImageDisableAllEvent extends SelectedImageEvent {
  const SelectedImageDisableAllEvent();
}
