part of 'stage_bloc.dart';

final class StageState extends Equatable {
  const StageState({
    this.worldResource = 'none',
    required this.worldName,
    required this.worldId,
    required this.resGroupId,
    required this.boundingRect,
    required this.pieces,
    required this.events,
    required this.pieceProperty,
    required this.eventProperty,
  });
  factory StageState.initailize() {
    return StageState(
      worldName: 'none',
      worldId: 0,
      resGroupId: 0,
      boundingRect: BoundingRect(
        x: 0,
        y: 0,
        width: MapConst.minBoundingWidth,
        height: MapConst.minBoundingHeight,
      ),
      pieces: HashMap(),
      events: HashMap(),
      pieceProperty: HashMap(),
      eventProperty: ItemProperty(),
    );
  }

  final String worldResource;
  final String worldName;
  final int worldId;
  final int resGroupId;
  final BoundingRect boundingRect;
  final HashMap<String, MapPieceItem> pieces;
  final HashMap<String, MapEventItem> events;
  final HashMap<(int, int), ItemProperty?> pieceProperty;
  final ItemProperty eventProperty;

  StageState copyWith({
    String? worldResource,
    String? worldName,
    int? worldId,
    int? resGroupId,
    BoundingRect? boundingRect,
    HashMap<String, MapPieceItem>? pieces,
    HashMap<String, MapEventItem>? events,
    Color? mapBoundingColor,
    HashMap<(int, int), ItemProperty>? pieceProperty,
    ItemProperty? eventProperty,
  }) {
    final newPieces = HashMap<String, MapPieceItem>();
    for (final e in (pieces ?? this.pieces).entries) {
      newPieces[e.key] = e.value.clone();
    }
    final newEvents = HashMap<String, MapEventItem>();
    for (final e in (events ?? this.events).entries) {
      newEvents[e.key] = e.value.clone();
    }
    final newPieceProperty = HashMap<(int, int), ItemProperty>();
    for (final e in (pieceProperty ?? this.pieceProperty).entries) {
      newPieceProperty[e.key] = e.value!.clone();
    }
    return StageState(
      worldResource: worldResource ?? this.worldResource,
      worldName: worldName ?? this.worldName,
      worldId: worldId ?? this.worldId,
      resGroupId: resGroupId ?? this.resGroupId,
      boundingRect: (boundingRect ?? this.boundingRect).clone(),
      pieces: newPieces,
      events: newEvents,
      pieceProperty: newPieceProperty,
      eventProperty: (eventProperty ?? this.eventProperty).clone(),
    );
  }

  @override
  List<Object?> get props => [
    worldResource,
    worldName,
    worldId,
    resGroupId,
    boundingRect,
    pieces,
    events,
    pieceProperty,
    eventProperty,
  ];
}
