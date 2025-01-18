part of 'canvas_bloc.dart';

sealed class CanvasEvent extends Equatable {
  const CanvasEvent();

  @override
  List<Object> get props => [];
}

final class InitailizeState extends CanvasEvent {
  const InitailizeState({required this.transformationController});

  final TransformationController transformationController;

  @override
  List<Object> get props => [transformationController];
}

final class InitCameraViewOffset extends CanvasEvent {
  const InitCameraViewOffset();

  @override
  List<Object> get props => [];
}

final class TransformControllerAddListener extends CanvasEvent {
  const TransformControllerAddListener({required this.listener});

  final void Function() listener;

  @override
  List<Object> get props => [];
}

final class UpdateMarqueeEvent extends CanvasEvent {
  const UpdateMarqueeEvent({this.start, this.end});

  final Offset? start, end;

  @override
  List<Object> get props => [];
}
