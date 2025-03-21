import 'dart:ui';

import 'package:equatable/equatable.dart';

enum BorderBackground { color, timeSpace }

class ConfigSetting {
  ConfigSetting({
    required this.playSingleFrame,
    required this.muteAudio,
    required this.plantCostume,
    required this.filterQuality,
  });

  factory ConfigSetting.fromJson(Map<String, dynamic> json) {
    return ConfigSetting(
      playSingleFrame: json['m_playSingleFrame'],
      muteAudio: json['m_muteAudio'],
      plantCostume: json['m_plantCostume'],
      filterQuality: FilterQuality.values[json['m_filterQuality']],
    );
  }
  bool playSingleFrame;
  bool muteAudio;
  bool plantCostume;
  FilterQuality filterQuality;

  static Map<String, dynamic> toJson(ConfigSetting data) {
    return {
      'm_playSingleFrame': data.playSingleFrame,
      'm_muteAudio': data.muteAudio,
      'm_plantCostume': data.plantCostume,
      'm_filterQuality': data.filterQuality.index,
    };
  }
}

class AnimationDetails {
  AnimationDetails({
    required this.animReplayDelayTimeMin,
    required this.animReplayDelayTimeMax,
    required this.usesRasterizedImagesInAnim,
  });

  factory AnimationDetails.fromJson(Map<String, dynamic> json) {
    return AnimationDetails(
      animReplayDelayTimeMin: double.parse('${json['AnimReplayDelayTimeMin']}'),
      animReplayDelayTimeMax: double.parse('${json['AnimReplayDelayTimeMax']}'),
      usesRasterizedImagesInAnim: json['UsesRasterizedImagesInAnim'],
    );
  }
  double? animReplayDelayTimeMin;
  double? animReplayDelayTimeMax;
  bool? usesRasterizedImagesInAnim;
}

class ConfigResource extends Equatable {
  const ConfigResource({
    required this.plant,
    required this.upgrade,
    required this.worldmap,
    required this.gameFeature,
    required this.narration,
    required this.tutorial,
  });

  factory ConfigResource.fromJson(Map<String, dynamic> json) {
    final worldMap = <String, Map<String, AnimationDetails>>{'none': {}};
    for (final worldName in (json['m_worldMap'] as Map<String, dynamic>).keys) {
      final value = json['m_worldMap'][worldName];
      final detailsList = <String, AnimationDetails>{};
      if (value['AnimationDetails'] != null) {
        for (final e in value['AnimationDetails']) {
          detailsList['anim${e['AnimNumber']}'] = AnimationDetails.fromJson(e);
        }
      }
      worldMap[worldName] = detailsList;
    }
    return ConfigResource(
      plant: json['m_plant'] as Map<String, dynamic>,
      upgrade: json['m_upgrade'] as Map<String, dynamic>,
      worldmap: worldMap,
      gameFeature:
          (json['m_gameFeature'] as List<dynamic>)
              .map((e) => e.toString())
              .toList(),
      narration:
          (json['m_narration'] as List<dynamic>)
              .map((e) => e.toString())
              .toList(),
      tutorial:
          (json['m_tutorial'] as List<dynamic>)
              .map((e) => e.toString())
              .toList(),
    );
  }
  final Map<String, dynamic> plant;
  final Map<String, dynamic> upgrade;
  final Map<String, Map<String, AnimationDetails>> worldmap;
  final List<String> gameFeature;
  final List<String> narration;
  final List<String> tutorial;

  @override
  List<Object> get props {
    return [plant, upgrade, worldmap, gameFeature, narration, tutorial];
  }
}

class ConfigModel extends Equatable {
  const ConfigModel({required this.setting, required this.resource});

  factory ConfigModel.fromJson(Map<String, dynamic> json) {
    return ConfigModel(
      setting: ConfigSetting.fromJson(json['setting']),
      resource: ConfigResource.fromJson(json['resource']),
    );
  }
  final ConfigSetting setting;
  final ConfigResource resource;

  @override
  List<Object> get props => [setting, resource];
}
