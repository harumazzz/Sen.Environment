part of '../wave.dart';

class DinoWave implements Wave {
  int row;

  String dinoType;

  DinoWave({
    required this.row,
    required this.dinoType,
  });

  DinoWave.withDefault()
      : row = 1,
        dinoType = 'raptor';

  void replaceWith({
    int? row,
    String? dinoType,
  }) {
    this.row = row ?? this.row;
    this.dinoType = dinoType ?? this.dinoType;
  }

  @override
  dynamic toJson(String aliasesName) {
    return {
      'aliases': [aliasesName],
      'objclass': 'DinoWaveActionProps',
      'objdata': {
        // Dino row starts at 0
        'DinoRow': row - 1,
        'DinoType': dinoType,
      }
    };
  }
}
