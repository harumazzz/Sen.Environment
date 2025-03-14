part of '../wave.dart';

class RegularWave implements Wave {
  RegularWave({required this.zombies}) : additionalPlantfood = 0;
  final List<Spawn> zombies;

  int additionalPlantfood;

  @override
  dynamic toJson(String aliasesName) {
    return {
      'aliases': [aliasesName],
      'objclass': 'SpawnZombiesJitteredWaveActionProps',
      'objdata': {
        'AdditionalPlantfood': additionalPlantfood,
        'Zombies':
            zombies
                .map(
                  (e) => {
                    'Row': e.row,
                    'Type': 'RTID(${e.typename}@ZombieTypes)',
                  },
                )
                .toList(),
      },
    };
  }
}

class Spawn {
  Spawn({required this.row, required this.typename});
  final int row;
  final String typename;
}
