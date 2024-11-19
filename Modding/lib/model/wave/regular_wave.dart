part of '../wave.dart';

class RegularWave implements Wave {
  final List<Spawn> zombies;

  int additionalPlantfood;

  RegularWave({
    required this.zombies,
  }) : additionalPlantfood = 0;

  @override
  dynamic toJson(String aliasesName) {
    return {
      'aliases': [aliasesName],
      'objclass': 'SpawnZombiesJitteredWaveActionProps',
      'objdata': {
        'AdditionalPlantfood': additionalPlantfood,
        'Zombies': zombies
            .map(
              (e) => {
                'Row': e.row,
                'Type': 'RTID(${e.typename}@ZombieTypes)',
              },
            )
            .toList(),
      }
    };
  }
}

class Spawn {
  final int row;
  final String typename;

  Spawn({
    required this.row,
    required this.typename,
  });
}
