part of '../wave.dart';

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
        eventName = 'sandstorm',
        groupSize = 1,
        zombies = [];
  @override
  dynamic toJson(String aliasesName) {
    return {
      'aliases': [aliasesName],
      'objclass': 'StormZombieSpawnerProps',
      'objdata': {
        'AdditionalPlantfood': '0',
        'Type': eventName,
        'ColumnStart': columnStart,
        'ColumnEnd': columnEnd,
        'TimeBetweenGroups': timeBetweenGroups,
        'GroupSize': groupSize,
        'Zombies': zombies
            .map(
              (e) => {
                'Type': 'RTID($e@ZombieTypes)',
              },
            )
            .toList(),
      },
    };
  }
}
