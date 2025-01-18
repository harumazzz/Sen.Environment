enum ActionType {
  // islands
  islandChangeID,
  islandScale,
  islandChangeLayer,
  islandChangeParallax,
  islandRotate,
  islandChangeRotationRate,
  islandArtFlip,
  // events,
  eventChangeID,
  eventChangeName,
  eventChangeParent,
  eventChangeUnlockedFrom,
  eventChangeVisibleFrom,
  eventChangeAutoVisible,
  eventChangeLevelData,
  eventChangeDisplayText,
  eventChangeNarUnlocked,
  eventChangeNarCompleted,
  eventChangeTutorialUnlocked,
  eventChangeLevelToggle,
  eventChangePlantType,
  eventChangeUpgradeType,
  eventChangeStarCost,
  eventChangeKeyCost,
  eventArtFlip,
  //
  moveItem,
  eraseItem,
  addItem,
  pasteItem,
  selectItem,
  rectangleSelect,
  deSelect,
  //
  mapChangeName,
  mapChangeWorldID,
  mapChangeResID,
  mapChangeBounding,
  //etc
  newMapEditor,
  openWorldMap,
  loadWorldResource,
  //
  createNewLayer,
  deleteLayer,
  mergeDownLayer,
  moveUpLayer,
  moveDownLayer
}

class ActionService<T> {
  ActionType actionType;

  Map<T, dynamic>? data;

  void Function(Map<T, dynamic>?) change;

  ActionService({required this.actionType, required this.change, this.data});

  void run() {
    change(data);
  }
}
