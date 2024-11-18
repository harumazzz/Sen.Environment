part of '../wave.dart';

class LevelModule {
  List<String>? notificationEvent;

  LevelModule({this.notificationEvent});

  LevelModule.fromJson(Map<String, dynamic> json) {
    notificationEvent = json['notificationEvent'].cast<String>();
  }

  Map<String, dynamic> toJson() {
    final Map<String, dynamic> data = new Map<String, dynamic>();
    data['notificationEvent'] = this.notificationEvent;
    return data;
  }
}
