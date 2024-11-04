import 'package:flutter/material.dart';
import 'package:sen/model/level.dart';
import 'package:sen/screen/level_maker/music_type.dart';

class LevelExchanger {
  final TextEditingController levelNameController;
  final TextEditingController levelDescriptionController;
  final TextEditingController levelStageController;
  final TextEditingController lawnMowerController;
  final bool hasSunFalling;

  final MusicType musicType;

  LevelExchanger({
    required this.levelNameController,
    required this.levelDescriptionController,
    required this.levelStageController,
    required this.lawnMowerController,
    required this.hasSunFalling,
    required this.musicType,
  });

  dynamic _buildLevelDefinition() {
    final result = {
      "objclass": "LevelDefinition",
      "objdata": {
        "Description": levelDescriptionController.text,
        "Loot": "RTID(DefaultLoot@LevelModules)",
        "Modules": [
          "RTID(StandardIntro@LevelModules)",
          "RTID(ZombiesDeadWinCon@LevelModules)",
          "RTID(DefaultZombieWinCondition@LevelModules)",
          "RTID(DefaultSunDropper@LevelModules)",
        ],
        "Name": levelNameController.text,
        "StageModule": 'RTID(${levelStageController.text}@LevelModules)',
      }
    };
    final lawnMower = lawnMowerController.text;
    if (lawnMower != 'SaveMowers') {
      ((result['objdata'] as dynamic)['Modules'] as List<dynamic>).add(
        "RTID($lawnMower@LevelModules)",
      );
    }
    if (hasSunFalling) {
      ((result['objdata'] as dynamic)['Modules'] as List<dynamic>).add(
        "RTID(DefaultSunDropper@LevelModules)",
      );
    }
    switch (musicType) {
      case MusicType.minigame:
        ((result['objdata'] as dynamic))['MusicType'] = 'MiniGame_B';
        break;
      case MusicType.ultimateBattle:
        ((result['objdata'] as dynamic))['MusicType'] = 'MiniGame_A';
        break;
      case MusicType.normal:
        break;
    }
    return result;
  }

  Level buildLevel() {
    return Level(
      version: 1,
      objects: [
        _buildLevelDefinition(),
      ],
    );
  }
}
