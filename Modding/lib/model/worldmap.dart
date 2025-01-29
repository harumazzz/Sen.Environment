const int kVersion = 1;

enum LevelNodeType { normal, minigame, miniboss, nonfinalboss, boss, danger }

enum WorldMapEventStatus { none, undiscovered, locked, unlocked, cleared }

enum EventType {
  none,
  doodad,
  // ignore: constant_identifier_names
  path_node,
  // ignore: constant_identifier_names
  star_gate,
  // ignore: constant_identifier_names
  key_gate,
  powerup,
  upgrade,
  plantbox,
  plant,
  giftbox,
  pinata,
  level,
  island
}

T getType<T extends Enum>(String name, List<T> values) {
  final eventType = values.firstWhere((e) => e.name.toLowerCase() == name.toLowerCase(), orElse: () => values.first);
  return eventType;
}

String? getName<T extends Enum>(T? type) {
  return type?.name;
}

bool checkRange(num min, num max, num value) {
  return value >= min && value < max;
}

class Position {
  double x;
  double y;

  Position({required this.x, required this.y});

  Position clone() {
    return Position(x: x, y: y);
  }

  factory Position.fromJson(Map<String, dynamic> json) {
    return Position(
      x: (json['x']).toDouble(),
      y: (json['y']).toDouble(),
    );
  }

  static Map<String, dynamic> toJson(Position data) {
    return {'x': data.x, 'y': data.y};
  }
}

enum PieceType { image, animation, invaild }

class MapPieceItem {
  int imageID;
  PieceType pieceType;
  Position position;
  int parallax;
  int layer;
  double scaleX;
  double scaleY;
  bool isArtFlipped;
  int rotationAngle;
  double rotationRate;

  MapPieceItem(
      {required this.imageID,
      required this.pieceType,
      required this.position,
      required this.parallax,
      required this.layer,
      required this.scaleX,
      required this.scaleY,
      required this.isArtFlipped,
      required this.rotationAngle,
      required this.rotationRate});

  MapPieceItem clone() {
    return MapPieceItem(
      imageID: imageID,
      pieceType: pieceType,
      position: position.clone(),
      parallax: parallax,
      layer: layer,
      scaleX: scaleX,
      scaleY: scaleY,
      isArtFlipped: isArtFlipped,
      rotationAngle: rotationAngle,
      rotationRate: rotationRate,
    );
  }

  factory MapPieceItem.fromJson(Map<String, dynamic> json) {
    var imageID = json['m_imageID'] ?? 0;
    var type = PieceType.invaild;
    if (checkRange(0, 100, imageID)) {
      type = PieceType.image;
    } else if (checkRange(100, 1000, imageID)) {
      type = PieceType.animation;
      imageID -= 100;
    }
    var isArtFlipped = json['m_isArtFlipped'] ?? false;
    if (isArtFlipped is String) {
      isArtFlipped = isArtFlipped.toLowerCase() == 'true';
    }
    return MapPieceItem(
      imageID: ++imageID,
      pieceType: type,
      position: Position.fromJson(json['m_position']),
      parallax: json['m_parallaxLayer'] ?? 0,
      layer: json['m_drawLayer'] ?? 0,
      scaleX: (json['m_scaleX'] ?? 1).toDouble(),
      scaleY: (json['m_scaleY'] ?? 1).toDouble(),
      isArtFlipped: isArtFlipped,
      rotationAngle: json['m_rotationAngle'] ?? 0,
      rotationRate: (json['m_rotationRate'] ?? 0.0).toDouble(),
    );
  }

  static Map<String, dynamic> toJson(MapPieceItem data) {
    var imageID = data.imageID;
    switch (data.pieceType) {
      case PieceType.animation:
        imageID += 100;
        break;
      default: //nothing to do
        break;
    }
    return {
      'm_eventType': 'island',
      'm_imageID': --imageID,
      'm_position': Position.toJson(data.position),
      'm_parallaxLayer': data.parallax,
      'm_drawLayer': data.layer,
      'm_scaleX': data.scaleX,
      'm_scaleY': data.scaleY,
      'm_isArtFlipped': data.isArtFlipped,
      'm_rotationAngle': data.rotationAngle,
      'm_rotationRate': data.rotationRate
    };
  }
}

class MapEventItem {
  EventType eventType;
  int eventID;
  Position position;
  String name;
  String? toggleName;
  String? dataString;
  String unlockedFrom;
  String visibleFrom;
  String parentEvent;
  String? displayText;
  String? unlockedNarrationID;
  String? completedNarrationID;
  bool autoVisible;
  bool? isArtFlipped;
  int? cost;
  String? worldMapTutorial;
  WorldMapEventStatus? worldMapTutorialVisibleWhen;
  LevelNodeType? levelNodeType;

  MapEventItem(
      {required this.eventType,
      required this.eventID,
      required this.position,
      required this.name,
      this.toggleName,
      this.dataString,
      required this.unlockedFrom,
      required this.visibleFrom,
      required this.parentEvent,
      required this.autoVisible,
      this.isArtFlipped,
      this.cost,
      this.displayText,
      this.unlockedNarrationID,
      this.completedNarrationID,
      this.worldMapTutorial,
      this.worldMapTutorialVisibleWhen,
      this.levelNodeType});

  MapEventItem clone() {
    return MapEventItem(
      eventType: eventType,
      eventID: eventID,
      position: position.clone(),
      name: name,
      toggleName: toggleName,
      dataString: dataString,
      unlockedFrom: unlockedFrom,
      visibleFrom: visibleFrom,
      parentEvent: parentEvent,
      autoVisible: autoVisible,
      isArtFlipped: isArtFlipped,
      cost: cost,
      displayText: displayText,
      unlockedNarrationID: unlockedNarrationID,
      completedNarrationID: completedNarrationID,
      worldMapTutorial: worldMapTutorial,
      worldMapTutorialVisibleWhen: worldMapTutorialVisibleWhen,
      levelNodeType: levelNodeType,
    );
  }

  factory MapEventItem.fromJson(Map<String, dynamic> json) {
    var eventID = json['m_eventId'] ?? 0;
    if (eventID is String) {
      eventID = int.tryParse(eventID) ?? 0;
    }
    return MapEventItem(
      eventType: getType(json['m_eventType'].toString(), EventType.values),
      eventID: eventID,
      position: Position.fromJson(json['m_position']),
      name: json['m_name'] ?? '',
      toggleName: json['m_toggleName'],
      dataString: json['m_dataString'],
      unlockedFrom: json['m_unlockedFrom'] ?? '',
      visibleFrom: json['m_visibleFrom'] ?? '',
      autoVisible: json['m_autoVisible'] ?? false,
      isArtFlipped: json['m_isArtFlipped'],
      cost: json['m_cost'],
      parentEvent: json['m_parentEvent'] ?? '',
      displayText: json['m_displayText'] ?? '',
      unlockedNarrationID: json['m_unlockedNarrationID'],
      completedNarrationID: json['m_completedNarrationID'],
      worldMapTutorial: json['m_worldMapTutorial'],
      worldMapTutorialVisibleWhen:
          getType(json['m_worldMapTutorialVisibleWhen'].toString(), WorldMapEventStatus.values),
      levelNodeType: getType(json['m_levelNodeType'].toString(), LevelNodeType.values),
    );
  }

  static Map<String, dynamic> toJson(MapEventItem data) {
    return {
      'm_eventType': getName(data.eventType),
      'm_eventId': data.eventID,
      'm_position': Position.toJson(data.position),
      'm_name': data.name,
      'm_toggleName': data.toggleName ?? '',
      'm_dataString': data.dataString ?? '',
      'm_unlockedFrom': data.unlockedFrom,
      'm_visibleFrom': data.visibleFrom,
      'm_autoVisible': data.autoVisible,
      'm_isArtFlipped': data.isArtFlipped,
      'm_cost': data.cost ?? 0,
      'm_parentEvent': data.parentEvent,
      'm_displayText': data.displayText ?? '',
      'm_unlockedNarrationID': data.unlockedNarrationID ?? '',
      'm_completedNarrationID': data.completedNarrationID ?? '',
      'm_worldMapTutorial': data.worldMapTutorial ?? '',
      'm_worldMapTutorialVisibleWhen': getName(data.worldMapTutorialVisibleWhen) ?? '',
      'm_levelNodeType': getName(data.levelNodeType) ?? '',
    };
  }
}

class BoundingRect {
  int x;
  int y;
  int width;
  int height;

  BoundingRect({required this.x, required this.y, required this.width, required this.height});

  BoundingRect clone() {
    return BoundingRect(
      x: x,
      y: y,
      width: width,
      height: height,
    );
  }

  factory BoundingRect.fromJson(Map<String, dynamic> json) {
    return BoundingRect(
      x: (json['mX']),
      y: (json['mY']),
      width: (json['mWidth']),
      height: (json['mHeight']),
    );
  }

  static Map<String, dynamic> toJson(BoundingRect data) {
    return {'mX': data.x, 'mY': data.y, 'mWidth': data.width, 'mHeight': data.height};
  }
}

class WorldData {
  String worldName;
  int worldID;
  int resGroupID;
  BoundingRect boundingRect;
  List<MapPieceItem> pieces;
  List<MapEventItem> events;
  int? creationTime;
  final int version = kVersion;

  WorldData(
      {required this.worldName,
      required this.worldID,
      required this.resGroupID,
      required this.boundingRect,
      required this.pieces,
      required this.events,
      this.creationTime});

  factory WorldData.fromJson(Map<String, dynamic> json) {
    return WorldData(
        worldName: json['m_worldName'],
        worldID: json['m_worldId'],
        resGroupID: json['m_resGroupID'],
        boundingRect: BoundingRect.fromJson(json['m_boundingRect']),
        pieces: (json['m_mapPieces'] as List).map((e) => MapPieceItem.fromJson(e)).toList(),
        events: (json['m_eventList'] as List).map((e) => MapEventItem.fromJson(e)).toList());
  }

  static Map<String, dynamic> toJson(WorldData data) {
    return {
      'm_worldName': data.worldName,
      'm_worldId': data.worldID,
      'm_resGroupID': data.resGroupID,
      'm_boundingRect': BoundingRect.toJson(data.boundingRect),
      'm_mapPieces': data.pieces.map((e) => MapPieceItem.toJson(e)).toList(),
      'm_eventList': data.events.map((e) => MapEventItem.toJson(e)).toList(),
      'm_creationTime': data.creationTime,
      'm_version': data.version
    };
  }
}

class WorldMap {
  WorldMap({required this.list});

  final int version = kVersion;
  List<WorldData> list;

  factory WorldMap.fromJson(Map<String, dynamic> json) {
    return WorldMap(
      list: (json['objects'] as List).map((e) {
        if (e['objclass'] != 'WorldData') {
          throw Exception('Invalid Worldmap data objclass');
        }
        return WorldData.fromJson(e['objdata']);
      }).toList(),
    );
  }

  static Map<String, dynamic> toJson(WorldMap data) {
    return {
      'version': data.version,
      'objects': data.list.map((e) {
        return {'objclass': 'WorldData', 'objdata': WorldData.toJson(e)};
      }).toList()
    };
  }
}
