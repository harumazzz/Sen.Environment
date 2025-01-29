import 'dart:ui';

enum BorderBackground { color, timeSpace, senLogo }

class ConfigSetting {
  bool playSingleFrame;
  bool muteAudio;
  bool plantCostume;
  FilterQuality filterQuality;
//  final dimension.Dimension renderingSize;

  ConfigSetting({
    required this.playSingleFrame,
    required this.muteAudio,
    required this.plantCostume,
    required this.filterQuality,
  });

  factory ConfigSetting.fromJson(Map<String, dynamic> json) {
    return ConfigSetting(
      playSingleFrame: (json['m_playSingleFrame']),
      muteAudio: (json['m_muteAudio']),
      plantCostume: (json['m_plantCostume']),
      filterQuality: FilterQuality.values[(json['m_filterQuality'])],
      /*
        renderingSize: dimension.Dimension(
            width: (json['m_renderingSize']['width']),
            height: (json['m_renderingSize']['height']))
            */
    );
  }

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
  final double animReplayDelayTimeMin;
  final double animReplayDelayTimeMax;
  final bool usesRasterizedImagesInAnim;

  const AnimationDetails(
      {required this.animReplayDelayTimeMin,
      required this.animReplayDelayTimeMax,
      required this.usesRasterizedImagesInAnim});

  factory AnimationDetails.fromJson(Map<String, dynamic> json) {
    return AnimationDetails(
      animReplayDelayTimeMin: double.parse('${json['AnimReplayDelayTimeMin']}'),
      animReplayDelayTimeMax: double.parse('${json['AnimReplayDelayTimeMax']}'),
      usesRasterizedImagesInAnim: json['UsesRasterizedImagesInAnim'],
    );
  }
}

class ConfigResource {
  final Map<String, dynamic> plant;
  final Map<String, dynamic> upgrade;
  final Map<String, Map<String, AnimationDetails>> worldmap;
  final List<String> gameFeature;
  final List<String> narration;
  final List<String> tutorial;

  const ConfigResource(
      {required this.plant,
      required this.upgrade,
      required this.worldmap,
      required this.gameFeature,
      required this.narration,
      required this.tutorial});

  factory ConfigResource.fromJson(Map<String, dynamic> json) {
    final worldMap = <String, Map<String, AnimationDetails>>{"none": {}};
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
        plant: (json['m_plant'] as Map<String, dynamic>),
        upgrade: (json['m_upgrade'] as Map<String, dynamic>),
        worldmap: worldMap,
        gameFeature: (json['m_gameFeature'] as List<dynamic>)
            .map((e) => e.toString())
            .toList(),
        narration: (json['m_narration'] as List<dynamic>)
            .map((e) => e.toString())
            .toList(),
        tutorial: (json['m_tutorial'] as List<dynamic>)
            .map((e) => e.toString())
            .toList());
  }

  /*
  static Map<String, dynamic> toJson(ConfigResource data) {
    data.worldmap.remove('none');
    return {
      'm_plant': data.plant,
      'm_upgrade': data.upgrade,
      'm_worldMap': data.worldmap,
      'm_gameFeature': data.gameFeature,
      'm_narration': data.narration,
      'm_tutorial': data.tutorial,
    };
  }
  */
}

class ConfigModel {
  final ConfigSetting setting;
  final ConfigResource resource;

  const ConfigModel({required this.setting, required this.resource});

  factory ConfigModel.fromJson(Map<String, dynamic> json) {
    return ConfigModel(
        setting: ConfigSetting.fromJson(json['setting']),
        resource: ConfigResource.fromJson(json['resource']));
  }

  /*
  static Map<String, dynamic> toJson(ConfigModel data) {
    return {
      'setting': ConfigSetting.toJson(data.setting),
      'resource': ConfigResource.toJson(data.resource),
    };
  }
  */
}
