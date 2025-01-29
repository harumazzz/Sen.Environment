part of 'canvas_bloc.dart';

final class CanvasState extends Equatable {
  const CanvasState({required this.canvasController});

  final CanvasController canvasController;

  factory CanvasState.initialize() {
    return CanvasState(
        canvasController: CanvasController(
            transformationController: TransformationController()));
  }

  @override
  List<Object?> get props => [canvasController];
}
