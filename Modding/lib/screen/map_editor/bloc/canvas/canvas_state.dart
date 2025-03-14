part of 'canvas_bloc.dart';

final class CanvasState extends Equatable {
  const CanvasState({required this.canvasController});
  factory CanvasState.initialize() {
    return CanvasState(
      canvasController: CanvasController(
        transformationController: TransformationController(),
      ),
    );
  }

  final CanvasController canvasController;

  @override
  List<Object?> get props => [canvasController];
}
