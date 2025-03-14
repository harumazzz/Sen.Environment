enum EventNodeType {
  firstLevel,
  normal,
  minigame,
  miniboss,
  nonfinalboss,
  boss,
  // zombossNode,
  hologramBoss,
  danger,
  pinata,
  packet,
  plant,
  giftbox,
  upgrade,
  keygate,
  keyGateLeft,
  stargate,
  stargateLeft,
  pathNode,
  doodad,
  mapPath,
}

const Map<EventNodeType, bool> eventAnimation = {
  EventNodeType.firstLevel: true,
  EventNodeType.normal: true,
  EventNodeType.minigame: true,
  EventNodeType.miniboss: true,
  EventNodeType.nonfinalboss: true,
  EventNodeType.boss: true,
  //  EventNodeType.zombossNode: true,
  EventNodeType.hologramBoss: true,
  EventNodeType.danger: true,
  EventNodeType.pinata: false,
  EventNodeType.packet: false,
  EventNodeType.plant: true,
  EventNodeType.giftbox: true,
  EventNodeType.upgrade: false,
  EventNodeType.keygate: true,
  EventNodeType.stargate: true,
  EventNodeType.pathNode: false,
  EventNodeType.doodad: false,
  EventNodeType.mapPath: true,
};

//$1 is lock, $2 is open
const Map<EventNodeType, (Iterable<String>, Iterable<String>)>
eventAnimationLabel = {
  EventNodeType.firstLevel: (['unlocked'], ['finished']),
  EventNodeType.normal: (['locked_idle'], ['finished']),
  EventNodeType.minigame: (['locked_idle'], ['finished']),
  EventNodeType.miniboss: (['unlocked'], ['finished']),
  EventNodeType.nonfinalboss: (['locked_idle'], ['finished']),
  EventNodeType.boss: (['active'], ['defeated']),
  // EventNodeType.zombossNode: (['active'], ['defeated']),
  EventNodeType.hologramBoss: (
    ['idle', 'idle2', 'laugh_broken', 'idle3', 'laugh', 'idle4'],
    ['defeated'],
  ),
  EventNodeType.plant: ([], ['idle', 'idle2', 'idle3', 'idle4']),
  EventNodeType.danger: (['locked_idle'], ['unlocked_idle']),
  EventNodeType.giftbox: (['idle'], ['open_idle']),
  EventNodeType.stargate: (['locked_right'], ['open_right']),
  EventNodeType.stargateLeft: (['inactive_left'], ['open_left']),
  EventNodeType.keygate: (['gate_right_locked'], ['gate_right_unlocked']),
  EventNodeType.keyGateLeft: (['gate_left_locked'], ['gate_left_unlocked']),
  EventNodeType.mapPath: (['beam_path_on'], ['beam_path_open']),
};

Iterable<String> getIdlePlay(String? plantType, Iterable<String> labelInfo) {
  final playLabelEvent =
      plantIdleLabel.containsKey(plantType)
          ? plantIdleLabel[plantType]!
          : eventAnimationLabel[EventNodeType.plant]!.$2;
  final label = <String>[];
  for (final e in playLabelEvent) {
    if (labelInfo.contains(e)) {
      label.add(e);
    }
  }
  return label;
}

Iterable<String> getCostumeSpriteDisable(
  String plantType,
  Iterable<String> spriteList, {
  bool enableCostume = true,
}) {
  final defaultSpriteList = <String>[];
  for (final e in spriteList) {
    if (e.toLowerCase().startsWith('custom_')) {
      defaultSpriteList.add(e);
    }
    if (!enableCostume) {
      if (e.toLowerCase().startsWith('_custom')) {
        defaultSpriteList.add(e);
      }
    }
  }
  final disableList = plantSpriteDisable[plantType] ?? ([], []);
  if (enableCostume) {
    final oldLength = defaultSpriteList.length;
    defaultSpriteList.removeWhere(
      (e) => disableList.$2.contains(e.toLowerCase()),
    );
    if (oldLength == defaultSpriteList.length) {
      final first = defaultSpriteList.firstWhere(
        (e) => e.toLowerCase().startsWith('custom_'),
        orElse: () => 'null',
      );
      defaultSpriteList.remove(first);
    }
  }
  for (final e in disableList.$1) {
    if (e.startsWith('rg*')) {
      final reg = RegExp(e.substring(3), caseSensitive: false);
      for (final sprite in spriteList) {
        if (reg.hasMatch(sprite)) {
          defaultSpriteList.add(sprite);
        }
      }
    } else {
      defaultSpriteList.add(e);
    }
  }
  return defaultSpriteList;
}

const Map<String, Iterable<String>> plantIdleLabel = {
  'gravebuster': ['attack1'],
  'chilibean': ['idle1', 'idle2'],
  'springbean': ['idle1', 'idle2'],
  'redstinger': ['idle1_1', 'idle1_2'],
  'peapod': ['idle5'],
  'sunshroom': ['idle_stage3', 'idle2_stage3'],
  'puffshroom': ['idle_stage1', 'idle2_stage1'],
  'chardguard': ['idle_leaves3', 'idle2_leaves3', 'idle3_leaves3'],
  'strawburst': ['stage3_idle', 'stage3_idle2', 'stage3_idle3'],
  'electricblueberry': ['idle2_1', 'idle2_2', 'idle2_3', 'idle2_4'],
  'escaperoot': ['idle_above', 'idle_above2'],
  'kiwibeast': ['idle_stage1_', 'idle_stage1_1', 'idle_stage1_2'],
  'caulipower': ['idle4_1'],
  'zoybeanpod': ['idle'],
  'filamint': ['loop'],
  'peppermint': ['loop'],
  'wintermint': ['loop'],
  'enlightenmint': ['loop'],
  'reinforcemint': ['loop'],
  'bombardmint': ['loop'],
  'ailmint': ['loop'],
  'enchantmint': ['loop'],
  'containmint': ['loop'],
  'enforcemint': ['loop'],
  'armamint': ['loop'],
  'concealmint': ['loop'],
  'spearmint': ['loop'],
  'appeasemint': ['loop'],
  'frostbonnet': ['idle1_ice', 'idle2_ice'],
  'iceweed': ['stage1_idle1', 'stage1_idle2'],
  'tumbleweed': ['idle1_1', 'idle1_2'],
};

/*
const Map<EventNodeType, Iterable<String>> eventAnimationSpriteDisable = {
  EventNodeType.plant: [
    '_custom',
    '_custom_02',
    '_custom_left',
    '_custom_right',
    'custom_01',
    'custom_02',
    'custom_03',
    'custom_04',
    'custom_05',
    'custom_06',
    'custom_07',
    'custom_08',
    'custom_09',
  ]
};
*/

//$1 to disable custom, $2 to enable custom
const Map<String, (Iterable<String>, Iterable<String>)> plantSpriteDisable = {
  'ready': ([r'rg*\b\w*custom\w*\b'], []),
  'wallnut': (['custom_crableg', r'rg*\b\w*armor\w*\b'], []),
  'cherry_bomb': ([], ['custom_02_left', 'custom_02_right']),
  'splitpea': ([], ['custom_01_halo', 'custom_01_horns']),
  'tallnut': (['custom_valentines', r'rg*\b\w*armor\w*\b'], []),
  'endurian': ([r'rg*\b\w*armor\w*\b'], []),
  'peanut': ([r'rg*\b\w*helmet\w*\b'], []),
  'primalwallnut': ([r'rg*\b\w*armor\w*\b'], []),
  'explodeonut': ([r'rg*\b\w*armor\w*\b'], []),
  'moonflower': ([r'rg*\b\w*dark\w*\b'], []),
  'shadowshroom': ([r'rg*\b\w*dark\w*\b'], []),
  'nightshade': ([r'rg*\b\w*dark\w*\b', r'rg*\b\w*pf\w*\b'], []),
  'dusklobber': ([r'rg*\b\w*dark\w*\b'], []),
  'grimrose': ([r'rg*\b\w*dark\w*\b'], []),
  'explodeovine': ([r'rg*\b\w*armor\w*\b'], []),
  'dragonbruit': ([r'rg*\b\w*dark\w*\b'], []),
};
