import 'dart:collection';

import 'package:equatable/equatable.dart';
import '../include/visual_animation.dart';
import '../include/visual_image.dart';

enum ImageCommonType {
  buttonHudBackNormal,
  buttonHudBackSelected,
  freePinata,
  freePinataOpen,
  missingArtPiece,
  spaceSpiral,
  spaceDust,
  readySeedBank,
  readyPacket,
  sprout,
  keygateFlag,
  infoIcon,
  doodad,
  pathNode,
}

enum AnimationCommonType {
  giftBox,
  levelNode,
  levelNodeGargantuar,
  levelNodeMinigame,
  mapPath,
  yetiIcon,
  zombossNodeHologram,
  missingArtPieceAnimation,
  stargate,
  sodRoll,
  collectedUpgradeEffect,
  readyPlant,
}

class GameResource extends Equatable {
  const GameResource({
    required this.commonImage,
    required this.commonAnimation,
    required this.uiUniverse,
    required this.seedBank,
    required this.packet,
    required this.plant,
    required this.upgrade,
  });

  final HashMap<ImageCommonType, VisualImage> commonImage;

  final HashMap<AnimationCommonType, VisualAnimation> commonAnimation;

  final HashMap<String, VisualImage> uiUniverse;

  final HashMap<String, VisualImage?> seedBank;

  final HashMap<String, VisualImage?> packet;

  final HashMap<String, VisualAnimation?> plant;

  final HashMap<String, VisualImage?> upgrade;

  @override
  List<Object> get props {
    return [
      commonImage,
      commonAnimation,
      uiUniverse,
      seedBank,
      packet,
      plant,
      upgrade,
    ];
  }
}
