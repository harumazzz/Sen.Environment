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
  const SelectedImageToggleEvent({required this.index});
  final int index;

  @override
  List<Object?> get props => [index];
}

final class SelectedImageAllocateEvent extends SelectedImageEvent {
  const SelectedImageAllocateEvent({required this.size});
  final int size;

  @override
  List<Object?> get props => [size];
}

final class SelectedImageEnableAllEvent extends SelectedImageEvent {
  const SelectedImageEnableAllEvent();
}

final class SelectedImageDisableAllEvent extends SelectedImageEvent {
  const SelectedImageDisableAllEvent();
}
