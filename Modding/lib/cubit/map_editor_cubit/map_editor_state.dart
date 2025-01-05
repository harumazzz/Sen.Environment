part of 'map_editor_cubit.dart';

class MapEditorState extends Equatable {
  final String resourceLocation;

  const MapEditorState({
    required this.resourceLocation,
  });

  factory MapEditorState.initialize() {
    return const MapEditorState(
      resourceLocation: '',
    );
  }

  @override
  bool operator ==(Object other) {
    if (identical(this, other)) return true;

    return other is MapEditorState && other.resourceLocation == resourceLocation;
  }

  @override
  int get hashCode => resourceLocation.hashCode;

  @override
  List<Object?> get props => [resourceLocation];

  MapEditorState copyWith({
    String? resourceLocation,
  }) {
    return MapEditorState(
      resourceLocation: resourceLocation ?? this.resourceLocation,
    );
  }

  @override
  String toString() => 'MapEditorState(resourceLocation: $resourceLocation)';
}
