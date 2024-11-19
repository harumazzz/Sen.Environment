part of '../wave.dart';

class JamWave implements Wave {
  String notificationEvent;

  JamWave({
    required this.notificationEvent,
  });

  JamWave.withDefault() : notificationEvent = 'jam_punk';

  void replaceWith({
    String? notificationEvent,
  }) {
    this.notificationEvent = notificationEvent ?? this.notificationEvent;
  }

  @override
  dynamic toJson(String aliasesName) {
    return {
      'aliases': [aliasesName],
      'objclass': 'SpawnZombiesJitteredWaveActionProps',
      'objdata': {
        'NotificationEvents': [notificationEvent]
      }
    };
  }
}
