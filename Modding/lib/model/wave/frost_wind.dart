part of '../wave.dart';

class FrostWind implements Wave {
  List<Wind> winds;

  FrostWind({
    required this.winds,
  });

  FrostWind.withDefault() : winds = [];

  FrostWind clone() {
    return FrostWind(
      winds: winds
          .map(
            (Wind wind) => Wind(
              row: wind.row,
              direction: wind.direction,
            ),
          )
          .toList(),
    );
  }

  @override
  dynamic toJson(String aliasesName) {
    return {
      'aliases': [aliasesName],
      'objclass': 'FrostWindWaveActionProps',
      'objdata': {
        'Winds': [
          ...winds.map(
            (Wind e) => e.toJson(),
          ),
        ],
      },
    };
  }
}

extension FrostWindReplaceWith on FrostWind {
  void replaceWith(FrostWind other) {
    winds
      ..clear()
      ..addAll(
        other.winds
            .map(
              (Wind wind) => Wind(
                row: wind.row,
                direction: wind.direction,
              ),
            )
            .toList(),
      );
  }
}

class Wind {
  String direction;

  /// Using row = [1,2,3,4,5] but game load [0,1,2,3,4]
  int row;
  Wind({
    required this.row,
    required this.direction,
  });

  dynamic toJson() {
    return {
      'Direction': direction,
      'Row': (row - 1).toString(),
    };
  }
}
