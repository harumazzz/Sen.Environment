part of 'level_maker_cubit.dart';

class LevelMakerState extends Equatable {
  final String resourceLocation;

  const LevelMakerState({
    required this.resourceLocation,
  });

  factory LevelMakerState.initialize() {
    return const LevelMakerState(resourceLocation: '');
  }

  LevelMakerState copyWith({
    String? resourceLocation,
  }) {
    return LevelMakerState(
      resourceLocation: resourceLocation ?? this.resourceLocation,
    );
  }

  @override
  String toString() => 'LevelMakerState(resourceLocation: $resourceLocation)';

  @override
  bool operator ==(Object other) {
    if (identical(this, other)) return true;

    return other is LevelMakerState && other.resourceLocation == resourceLocation;
  }

  @override
  int get hashCode => resourceLocation.hashCode;

  @override
  List<Object?> get props => [resourceLocation];
}
