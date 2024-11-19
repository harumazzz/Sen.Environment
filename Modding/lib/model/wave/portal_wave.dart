part of '../wave.dart';

class PortalWave implements Wave {
  int column;
  int row;
  String type;

  PortalWave({
    required this.column,
    required this.row,
    required this.type,
  });

  PortalWave.withDefault()
      : column = 0,
        row = 0,
        type = 'egypt';

  void replaceWith({
    int? column,
    int? row,
    String? type,
  }) {
    this.column = column ?? this.column;
    this.row = row ?? this.row;
    this.type = type ?? this.type;
  }

  @override
  dynamic toJson(String aliasesName) {
    return {
      'aliases': [aliasesName],
      'objclass': 'SpawnModernPortalsWaveActionProps',
      'objdata': {
        'PortalColumn': column,
        'PortalRow': row,
        'PortalType': type,
      },
    };
  }
}
