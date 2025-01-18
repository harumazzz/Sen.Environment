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
  plant,
  giftbox,
  upgrade,
  keygate,
  keyGateLeft,
  stargate,
  stargateLeft,
  pathNode,
  doodad,
  mapPath
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
  EventNodeType.plant: false,
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
    ['defeated']
  ),
  EventNodeType.danger: (['locked_idle'], ['unlocked_idle']),
  EventNodeType.giftbox: (['idle'], ['open_idle']),
  EventNodeType.stargate: (['locked_right'], ['open_right']),
  EventNodeType.stargateLeft: (['inactive_left'], ['open_left']),
  EventNodeType.keygate: (['gate_right_locked'], ['gate_right_unlocked']),
  EventNodeType.keyGateLeft: (['gate_left_locked'], ['gate_left_unlocked']),
  EventNodeType.mapPath: (['beam_path_on'], ['beam_path_open']),
};
