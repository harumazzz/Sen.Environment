part of '../wave.dart';

class SpiderRain implements Wave {
  int columnStart;

  int columnEnd;

  int groupSize;

  double timeBetweenGroups;

  double fallTime;

  String typename;

  int count;

  String message;

  SpiderRain({
    required this.columnStart,
    required this.columnEnd,
    required this.groupSize,
    required this.timeBetweenGroups,
    required this.fallTime,
    required this.typename,
    required this.count,
    required this.message,
  });

  SpiderRain.withDefault()
      : columnStart = 5,
        columnEnd = 6,
        timeBetweenGroups = 0.2,
        fallTime = 1.5,
        groupSize = 1,
        count = 3,
        message = 'Spider Rain!',
        typename = 'future_imp';

  void replaceWith({
    int? columnStart,
    int? columnEnd,
    int? groupSize,
    double? timeBetweenGroups,
    double? fallTime,
    String? typename,
    int? count,
    String? message,
  }) {
    this.columnStart = columnStart ?? this.columnStart;
    this.columnEnd = columnEnd ?? this.columnEnd;
    this.groupSize = groupSize ?? this.groupSize;
    this.timeBetweenGroups = timeBetweenGroups ?? this.timeBetweenGroups;
    this.fallTime = fallTime ?? this.fallTime;
    this.typename = typename ?? this.typename;
    this.count = count ?? this.count;
    this.message = message ?? this.message;
  }

  @override
  dynamic toJson(String aliasesName) {
    return {
      'aliases': [aliasesName],
      'objclass': 'SpiderRainZombieSpawnerProps',
      'objdata': {
        'ColumnStart': columnStart,
        'ColumnEnd': columnEnd,
        'GroupSize': groupSize,
        'TimeBetweenGroups': timeBetweenGroups.toString(),
        'ZombieFallTime': fallTime.toString(),
        'SpiderZombieName': typename,
        'SpiderCount': count,
        'WaveStartMessage': message,
      },
    };
  }
}
