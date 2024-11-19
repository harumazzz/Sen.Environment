part of '../wave.dart';

class GroundSpawn implements Wave {
  int columnStart;

  int columnEnd;

  int additionalPlantFood;

  List<Spawn> zombies;

  GroundSpawn({
    required this.columnStart,
    required this.columnEnd,
    required this.additionalPlantFood,
    required this.zombies,
  });

  GroundSpawn.withDefault()
      : columnStart = 0,
        columnEnd = 0,
        additionalPlantFood = 0,
        zombies = [];

  void replaceWith({
    int? columnStart,
    int? columnEnd,
    int? additionalPlantFood,
    List<Spawn>? zombies,
  }) {
    this.columnStart = columnStart ?? this.columnStart;
    this.columnEnd = columnEnd ?? this.columnEnd;
    this.additionalPlantFood = additionalPlantFood ?? this.additionalPlantFood;
    this.zombies = zombies ?? this.zombies;
  }

  @override
  dynamic toJson(String aliasesName) {
    return {
      'aliases': [aliasesName],
      'objclass': 'SpawnZombiesFromGroundSpawnerProps',
      'objdata': {
        'AdditionalPlantfood': additionalPlantFood.toString(),
        'ColumnStart': columnStart,
        'ColumnEnd': columnEnd,
        'Zombies': zombies
            .map(
              (e) => {
                // PvZ2 Row count from 1
                'Row': (e.row + 1).toString(),
                'Type': 'RTID(${e.typename.toString()}@ZombieTypes)',
              },
            )
            .toList(),
      },
    };
  }
}
