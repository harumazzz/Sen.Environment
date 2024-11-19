part of '../wave.dart';

class Necromancy implements Wave {
  String message;
  int spawnWaitTime;
  bool suppressActionIfNoGridItemsFound;
  int additionalPlantFood;
  List<String> gridTypes;
  List<String> zombies;

  Necromancy({
    required this.message,
    required this.spawnWaitTime,
    required this.suppressActionIfNoGridItemsFound,
    required this.additionalPlantFood,
    required this.gridTypes,
    required this.zombies,
  });

  Necromancy.withDefault()
      : message = 'Necromancy!',
        spawnWaitTime = 0,
        suppressActionIfNoGridItemsFound = false,
        additionalPlantFood = 0,
        gridTypes = [],
        zombies = [];

  void replaceWith({
    String? message,
    int? spawnWaitTime,
    bool? suppressActionIfNoGridItemsFound,
    int? additionalPlantFood,
    List<String>? gridTypes,
    List<String>? zombies,
  }) {
    this.message = message ?? this.message;
    this.spawnWaitTime = spawnWaitTime ?? this.spawnWaitTime;
    this.suppressActionIfNoGridItemsFound = suppressActionIfNoGridItemsFound ??
        this.suppressActionIfNoGridItemsFound;
    this.additionalPlantFood = additionalPlantFood ?? this.additionalPlantFood;
    this.gridTypes = gridTypes ?? this.gridTypes;
    this.zombies = zombies ?? this.zombies;
  }

  @override
  dynamic toJson(String aliasesName) {
    return {
      'aliases': [aliasesName],
      'objclass': 'SpawnZombiesFromGridItemSpawnerProps',
      'objdata': {
        'WaveStartMessage': message,
        'ZombieSpawnWaitTime': spawnWaitTime,
        'SuppressActionIfNoGridItemsFound': suppressActionIfNoGridItemsFound,
        'AdditionalPlantfood': additionalPlantFood.toString(),
        'GridTypes': gridTypes
            .map(
              (e) => 'RTID($e@GridItemTypes)',
            )
            .toList(),
        'Zombies': zombies
            .map(
              (e) => ({
                'Type': 'RTID($e@ZombieTypes)',
              }),
            )
            .toList(),
      },
    };
  }
}
