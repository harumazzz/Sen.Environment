part of 'layer_bloc.dart';

enum RootLayerType { pieces, events }

final class LayerState extends Equatable {
  const LayerState({required this.treeController});

  final TreeController<LayerNode> treeController;

  factory LayerState.initailize() {
    return LayerState(
        treeController: TreeController(
      roots: [],
      childrenProvider: (LayerNode node) => node.children.values,
    ));
  }

  LayerState copyWith({TreeController<LayerNode>? treeController}) {
    return LayerState(treeController: treeController ?? this.treeController);
  }

  @override
  List<Object?> get props => [treeController.roots];
}
