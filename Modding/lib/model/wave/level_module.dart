part of '../wave.dart';

class LevelModule {
  List<String>? notificationEvent;
  List<String>? portal;
  List<String>? stage;
  List<String>? mower;

  LevelModule({
    this.notificationEvent,
    this.portal,
    this.stage,
    this.mower,
  });

  LevelModule.fromJson(Map<String, dynamic> json) {
    notificationEvent = json['notificationEvent'].cast<String>();
    portal = json['portal'].cast<String>();
    stage = json['stage'].cast<String>();
    mower = json['mower'].cast<String>();
  }

  Map<String, dynamic> toJson() {
    final Map<String, dynamic> data = <String, dynamic>{};
    data['notificationEvent'] = notificationEvent;
    data['portal'] = portal;
    data['stage'] = stage;
    data['mower'] = mower;
    return data;
  }
}
