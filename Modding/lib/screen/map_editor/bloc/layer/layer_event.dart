part of 'layer_bloc.dart';

sealed class LayerEvent extends Equatable {
  const LayerEvent();

  @override
  List<Object> get props => [];
}

final class InitailizeTreeController extends LayerEvent {
  const InitailizeTreeController();
  @override
  List<Object> get props => [];
}

final class InitailizeLayerNode extends LayerEvent {
  const InitailizeLayerNode();

  @override
  List<Object> get props => [];
}

final class UpdateNodeParallax extends LayerEvent {
  const UpdateNodeParallax({required this.parallax});

  final int parallax;

  @override
  List<Object> get props => [parallax];
}

final class ClearLayerNode extends LayerEvent {
  const ClearLayerNode();

  @override
  List<Object> get props => [];
}
