part of '../wave.dart';

class LevelModule {
  List<String>? notificationEvent;
  List<String>? portal;

  LevelModule({
    this.notificationEvent,
    this.portal,
  });

  LevelModule.fromJson(Map<String, dynamic> json) {
    notificationEvent = json['notificationEvent'].cast<String>();
    portal = json['portal'].cast<String>();
  }

  Map<String, dynamic> toJson() {
    final Map<String, dynamic> data = <String, dynamic>{};
    data['notificationEvent'] = notificationEvent;
    data['portal'] = portal;
    return data;
  }
}
