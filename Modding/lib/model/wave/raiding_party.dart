part of '../wave.dart';

class RaidingParty implements Wave {
  int count;
  int groupSize;
  double delayBetweenGroup;

  RaidingParty({
    required this.count,
    required this.groupSize,
    required this.delayBetweenGroup,
  });

  RaidingParty.withDefault()
      : count = 1,
        groupSize = 1,
        delayBetweenGroup = 0.5;

  void replaceWith({
    int? count,
    int? groupSize,
    double? delayBetweenGroup,
  }) {
    this.count = count ?? this.count;
    this.groupSize = groupSize ?? this.groupSize;
    this.delayBetweenGroup = delayBetweenGroup ?? this.delayBetweenGroup;
  }

  @override
  dynamic toJson(String aliasesName) {
    return {
      'aliases': [aliasesName],
      'objclass': 'RaidingPartyZombieSpawnerProps',
      'objdata': {
        'SwashbucklerCount': count,
        'GroupSize': groupSize,
        'TimeBetweenGroups': delayBetweenGroup,
      },
    };
  }
}
