abstract class Wave {
  dynamic toJson(String aliasesName);
}

class Spawn {
  final int row;
  final String typename;

  Spawn({
    required this.row,
    required this.typename,
  });
}

class RegularWave implements Wave {
  final List<Spawn> zombies;

  int additionalPlantfood;

  RegularWave({
    required this.zombies,
  }) : additionalPlantfood = 0;

  @override
  dynamic toJson(String aliasesName) {
    return {
      "aliases": [aliasesName],
      "objclass": "SpawnZombiesJitteredWaveActionProps",
      "objdata": {
        "AdditionalPlantfood": additionalPlantfood,
        "Zombies": zombies.map(
          (e) => {
            "Row": e.row,
            "Type": "RTID(${e.typename}@ZombieTypes)",
          },
        ),
      }
    };
  }
}

class LowTide implements Wave {
  int xStart;
  int xEnd;
  int groupSize;
  double delay;
  String message;
  String zombieType;
  int count;

  LowTide({
    required this.xStart,
    required this.xEnd,
    required this.groupSize,
    required this.delay,
    required this.message,
    required this.zombieType,
    required this.count,
  });

  LowTide.withDefault()
      : xStart = 5,
        xEnd = 6,
        groupSize = 1,
        delay = 0.05,
        message = 'Low Tide!',
        zombieType = 'tutorial',
        count = 1;

  void replaceWith({
    int? xStart,
    int? xEnd,
    int? groupSize,
    double? delay,
    String? message,
    String? zombieType,
    int? count,
  }) {
    this.xStart = xStart ?? this.xStart;
    this.xEnd = xEnd ?? this.xEnd;
    this.groupSize = groupSize ?? this.groupSize;
    this.delay = delay ?? this.delay;
    this.message = message ?? this.message;
    this.zombieType = zombieType ?? this.zombieType;
    this.count = count ?? this.count;
  }

  @override
  dynamic toJson(String aliasesName) {
    return {
      "aliases": [aliasesName],
      "objclass": "BeachStageEventZombieSpawnerProps",
      "objdata": {
        "ColumnStart": xStart,
        "ColumnEnd": xEnd,
        "ZombieCount": count,
        "GroupSize": groupSize,
        "TimeBetweenGroups": delay.toString(),
        "ZombieName": zombieType,
        "WaveStartMessage": message,
      },
    };
  }
}

class StormEvent implements Wave {
  int additionalPlantfood;
  int columnStart;
  int columnEnd;
  double timeBetweenGroups;
  int groupSize;
  List<String> zombies;
  String eventName;

  StormEvent({
    required this.additionalPlantfood,
    required this.columnStart,
    required this.columnEnd,
    required this.timeBetweenGroups,
    required this.groupSize,
    required this.zombies,
    required this.eventName,
  });

  void replaceWith({
    int? additionalPlantfood,
    int? columnStart,
    int? columnEnd,
    double? timeBetweenGroups,
    int? groupSize,
    List<String>? zombies,
    String? eventName,
  }) {
    this.additionalPlantfood = additionalPlantfood ?? this.additionalPlantfood;
    this.columnStart = columnStart ?? this.columnStart;
    this.columnEnd = columnEnd ?? this.columnEnd;
    this.timeBetweenGroups = timeBetweenGroups ?? this.timeBetweenGroups;
    this.groupSize = groupSize ?? this.groupSize;
    this.zombies = zombies ?? this.zombies;
    this.eventName = eventName ?? this.eventName;
  }

  StormEvent.withDefault()
      : additionalPlantfood = 0,
        columnStart = 0,
        columnEnd = 0,
        timeBetweenGroups = 0.5,
        eventName = "sandstorm",
        groupSize = 1,
        zombies = [];
  @override
  dynamic toJson(String aliasesName) {
    return {
      "aliases": [aliasesName],
      "objclass": "StormZombieSpawnerProps",
      "objdata": {
        "AdditionalPlantfood": "0",
        "Type": eventName,
        "ColumnStart": columnStart,
        "ColumnEnd": columnEnd,
        "TimeBetweenGroups": timeBetweenGroups,
        "GroupSize": groupSize,
        "Zombies": zombies.map(
          (e) => {
            "Type": "RTID($e@ZombieTypes)",
          },
        ),
      },
    };
  }
}
