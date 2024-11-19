part of '../wave.dart';

class TidalChange implements Wave {
  int changeAmount;

  TidalChange({
    required this.changeAmount,
  });

  TidalChange.withDefault() : changeAmount = 1;

  void replaceWith({
    int? changeAmount,
  }) {
    this.changeAmount = changeAmount ?? this.changeAmount;
  }

  @override
  dynamic toJson(String aliasesName) {
    return {
      'aliases': [aliasesName],
      'objclass': 'TidalChangeWaveActionProps',
      'objdata': {
        'TidalChange': {
          'ChangeAmount': 9 - changeAmount,
          'ChangeType': 'absolute',
        }
      }
    };
  }
}
