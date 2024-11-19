part of '../wave.dart';

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
      'aliases': [aliasesName],
      'objclass': 'BeachStageEventZombieSpawnerProps',
      'objdata': {
        'ColumnStart': xStart,
        'ColumnEnd': xEnd,
        'ZombieCount': count,
        'GroupSize': groupSize,
        'TimeBetweenGroups': delay.toString(),
        'ZombieName': zombieType,
        'WaveStartMessage': message,
      },
    };
  }
}
