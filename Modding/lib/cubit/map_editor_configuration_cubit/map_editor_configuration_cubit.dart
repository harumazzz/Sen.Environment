import 'dart:async';
import 'dart:collection';

import 'package:audioplayers/audioplayers.dart';
import 'package:custom_mouse_cursor/custom_mouse_cursor.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../model/animation.dart';
import '../../model/item.dart';
import '../../screen/map_editor/bloc/toolbar/toolbar_bloc.dart';
import '../../screen/map_editor/bloc/section/section_bloc.dart';
import '../../screen/map_editor/include/visual_animation.dart';
import '../../screen/map_editor/include/visual_image.dart';
import '../../screen/map_editor/models/action_service.dart';
import '../../screen/map_editor/models/config.dart';
import '../../screen/map_editor/models/editor_resource.dart';
import '../../screen/map_editor/models/event_node.dart';
import '../../screen/map_editor/models/game_resource.dart';
import '../../service/file_helper.dart';
import '../../i18n/app_localizations.dart';
import '../../service/ui_helper.dart';

part 'map_editor_configuration_state.dart';

class MapEditorConfigurationCubit extends Cubit<MapEditorConfigurationState> {
  MapEditorConfigurationCubit() : super(MapEditorConfigurationState());

  void initializeState() => emit(MapEditorConfigurationState());

  void _setState(MapEditorConfigurationState state) => emit(state);

  void _throwErrorState(String ex) {
    emit(
      MapEditorConfigurationState(
        status: AppConfigurationStatus.failed,
        errorSnapShot: ex,
      ),
    );
  }

  void reset() {
    emit(MapEditorConfigurationState());
  }

  Future<ConfigModel> _loadConfig(AppLocalizations los, String path) async {
    return ConfigModel.fromJson(await FileHelper.readJsonAsync(source: path));
  }

  static const sounds = [
    'grab2.mp3',
    'shell_hit.mp3',
    'smb2_throw.mp3',
    'level_select.mp3',
    'smash.mp3',
    'has_item.mp3',
  ];

  Future<EditorResource> _loadEditorResource(
    AppLocalizations los,
    String path,
  ) async {
    final eraseCursor = await UIHelper.ofDesktop(
      builder: () async {
        return await CustomMouseCursor.icon(
          Symbols.ink_eraser,
          size: 28,
          color: Colors.white,
        );
      },
    );
    final panCursor = await UIHelper.ofDesktop(
      builder: () async {
        return await CustomMouseCursor.icon(
          Symbols.pan_tool,
          size: 28,
          color: Colors.white,
        );
      },
    );
    final multiSelectCursor = await UIHelper.ofDesktop(
      builder: () async {
        return await CustomMouseCursor.icon(
          Symbols.pan_tool_alt,
          size: 28,
          color: Colors.white,
        );
      },
    );
    late AudioPlayer pickItemSound;
    late AudioPlayer removeItemSound;
    late AudioPlayer setItemSound;
    late AudioPlayer mapLoadedSound;
    late AudioPlayer clearMapSound;
    late AudioPlayer switchResourceSound;
    Future<void> loadSounds(String path) async {
      try {
        final players = List.generate(sounds.length, (_) => AudioPlayer());

        await Future.wait(
          players.asMap().entries.map((entry) async {
            final index = entry.key;
            final player = entry.value;
            final soundPath = '$path/sound/${sounds[index]}';

            final bytes = await FileHelper.readBufferAsync(source: soundPath);
            await player.setSourceBytes(bytes);
            await player.setReleaseMode(ReleaseMode.stop);
          }),
        );
        pickItemSound = players[0];
        removeItemSound = players[1];
        setItemSound = players[2];
        mapLoadedSound = players[3];
        clearMapSound = players[4];
        switchResourceSound = players[5];
      } catch (e) {
        throw Exception('Failed to load sounds: $e');
      }
    }

    await loadSounds(path);
    return EditorResource(
      eraseCursor: eraseCursor,
      panCursor: panCursor,
      multiSelectCursor: multiSelectCursor,
      pickItemSound: pickItemSound,
      removeItemSound: removeItemSound,
      setItemSound: setItemSound,
      mapLoadedSound: mapLoadedSound,
      clearMapSound: clearMapSound,
      switchResourceSound: switchResourceSound,
    );
  }

  Future<VisualImage?> loadVisualImage(String path) async {
    try {
      final data = await FileHelper.readBufferAsync(source: path);
      final completer = Completer<VisualImage>();
      final listener = ImageStreamListener((info, _) {
        completer.complete(info.image);
      });
      final stream = MemoryImage(data).resolve(const ImageConfiguration())
        ..addListener(listener);
      final image = await completer.future;
      stream.removeListener(listener);
      return image;
    } catch (ex) {
      return null;
    }
  }

  Future<VisualAnimation?> loadVisualAnimation(
    String path, {
    FilterQuality? filterQuality,
  }) async {
    try {
      final animationPath = '$path/animation.pam.json';
      final mediaPath = '$path/media';
      final visual = await VisualAnimation.create(
        animationPath,
        mediaPath,
        filterQuality: filterQuality,
      );
      return visual;
    } catch (ex) {
      return null;
    }
  }

  bool isReady() {
    return state.status == AppConfigurationStatus.success;
  }

  Future<VisualAnimation?> loadPlantVisualAnimation(
    String path,
    String plantType, {
    required bool enableCostume,
    FilterQuality? filterQuality,
  }) async {
    try {
      final animationPath = '$path/animation.pam.json';
      final mediaPath = '$path/media';
      final animation = SexyAnimation.fromJson(
        await FileHelper.readJsonAsync(source: animationPath),
      );
      final spriteDisable = getCostumeSpriteDisable(
        plantType.toLowerCase(),
        animation.sprite.map((e) => e.name),
        enableCostume: enableCostume,
      );
      final visual = await VisualAnimation.createByAnimation(
        animation,
        mediaPath,
        filterQuality: filterQuality,
        spriteDisable: spriteDisable,
      );
      return visual;
    } catch (ex) {
      return null;
    }
  }

  static const imagePaths = [
    (
      type: ImageCommonType.missingArtPiece,
      path: 'alwaysloaded/missing_artpiece.png',
    ),
    (type: ImageCommonType.readySeedBank, path: 'packet/ready.png'),
    (type: ImageCommonType.readyPacket, path: 'packet/sunflower.png'),
    (type: ImageCommonType.spaceSpiral, path: 'alwaysloaded/space_spiral.png'),
    (type: ImageCommonType.spaceDust, path: 'alwaysloaded/space_dust.png'),
    (type: ImageCommonType.freePinata, path: 'pinata/pinata_free_spine.png'),
    (
      type: ImageCommonType.freePinataOpen,
      path: 'pinata/pinatas_dust_spine_free.png',
    ),
    (
      type: ImageCommonType.buttonHudBackNormal,
      path: 'common/buttons_hud_back_normal.png',
    ),
    (
      type: ImageCommonType.buttonHudBackSelected,
      path: 'common/buttons_hud_back_selected.png',
    ),
    (type: ImageCommonType.keygateFlag, path: 'common/keygate_flag.png'),
    (type: ImageCommonType.infoIcon, path: 'common/info_icon.png'),
    (type: ImageCommonType.sprout, path: 'common/sprout.png'),
    (type: ImageCommonType.doodad, path: 'common/doodad1.png'),
    (type: ImageCommonType.pathNode, path: 'common/grass_light.png'),
  ];

  static const animationPaths = [
    (type: AnimationCommonType.giftBox, path: 'common/giftbox_world_map'),
    (type: AnimationCommonType.levelNode, path: 'common/level_node'),
    (
      type: AnimationCommonType.levelNodeGargantuar,
      path: 'common/level_node_gargantuar',
    ),
    (
      type: AnimationCommonType.levelNodeMinigame,
      path: 'common/level_node_minigame',
    ),
    (type: AnimationCommonType.mapPath, path: 'common/map_path'),
    (type: AnimationCommonType.yetiIcon, path: 'common/yeti_icon'),
    (
      type: AnimationCommonType.zombossNodeHologram,
      path: 'common/zomboss_node_hologram',
    ),
    (
      type: AnimationCommonType.missingArtPieceAnimation,
      path: 'alwaysloaded/missing_artpiece',
    ),
    (type: AnimationCommonType.stargate, path: 'common/stargate'),
    (type: AnimationCommonType.sodRoll, path: 'common/sod_roll'),
    (
      type: AnimationCommonType.collectedUpgradeEffect,
      path: 'common/collected_upgrade_effect',
    ),
  ];

  Future<GameResource> _loadGameResource(
    AppLocalizations los,
    String path,
    ConfigModel config,
  ) async {
    final filterQuality = config.setting.filterQuality;
    final commonImage = HashMap<ImageCommonType, VisualImage>();
    final commonAnimation = HashMap<AnimationCommonType, VisualAnimation>();
    final uiUniverse = HashMap<String, VisualImage>();
    final seedBank = HashMap<String, VisualImage?>();
    final packet = HashMap<String, VisualImage?>();
    final plant = HashMap<String, VisualAnimation?>();
    final upgrade = HashMap<String, VisualImage?>();

    Future<VisualImage> loadImage(String relativePath, String errorMsg) async {
      final img = await loadVisualImage('$path/$relativePath');
      if (img == null) {
        throw Exception(errorMsg);
      }
      return img;
    }

    Future<VisualAnimation> loadAnimation(
      String relativePath,
      String errorMsg,
    ) async {
      final anim = await loadVisualAnimation(
        '$path/$relativePath',
        filterQuality: filterQuality,
      );
      if (anim == null) {
        throw Exception(errorMsg);
      }
      return anim;
    }

    for (final entry in imagePaths) {
      commonImage[entry.type] = await loadImage(
        entry.path,
        los.cannot_load_missing_artpiece,
      );
    }

    for (final entry in animationPaths) {
      commonAnimation[entry.type] = await loadAnimation(
        entry.path,
        los.cannot_load_map_path,
      );
    }

    commonAnimation[AnimationCommonType.readyPlant] =
        await loadPlantVisualAnimation(
          '$path/plant/sunflower',
          filterQuality: filterQuality,
          'ready',
          enableCostume: config.setting.plantCostume,
        ) ??
        (throw Exception(los.cannot_load_ready_plant));

    for (final mapName in config.resource.worldmap.keys) {
      uiUniverse[mapName] =
          await loadVisualImage(
            '$path/alwaysloaded/ui_universe/$mapName.png',
          ) ??
          commonImage[ImageCommonType.missingArtPiece]!;
    }

    for (final plantName in config.resource.plant.keys) {
      final seedBankName = config.resource.plant[plantName];
      final image = await loadVisualImage('$path/packet/$seedBankName.png');
      seedBank.putIfAbsent(seedBankName, () => image);
      packet[plantName] = await loadVisualImage('$path/packet/$plantName.png');
    }

    for (final upgradeName in config.resource.upgrade.keys) {
      final src = config.resource.upgrade[upgradeName];
      upgrade[upgradeName] = await loadVisualImage(
        '$path/upgrade/upgrade_$src.png',
      );
    }

    return GameResource(
      commonImage: commonImage,
      commonAnimation: commonAnimation,
      uiUniverse: uiUniverse,
      seedBank: seedBank,
      packet: packet,
      plant: plant,
      upgrade: upgrade,
    );
  }

  HashMap<ToolType, Item> _initailizeTool(AppLocalizations los) {
    final toolItem = HashMap<ToolType, Item>.of({
      ToolType.openFile: Item(
        title: '${los.open} (Ctrl + O)',
        description: los.open_description,
        icon: Symbols.file_open,
      ),
      ToolType.saveFile: Item(
        title: '${los.save} (Ctrl + S)',
        description: los.save_description,
        icon: Symbols.save,
      ),
      ToolType.rectangleTool: Item(
        title: '${los.rectangle_tool} (Ctrl Left)',
        description: los.select_multiple_items_by_rectangle,
        icon: Symbols.gesture_select,
      ),
      ToolType.eraseTool: Item(
        title: '${los.eraser} (E)',
        description: los.eraser_description,
        icon: Symbols.ink_eraser,
      ),
      ToolType.panTool: Item(
        title: '${los.pan_tool} (Space)',
        description: los.pan_tool_description,
        icon: Symbols.pan_tool,
      ),
      ToolType.resizeTool: Item(
        title: '${los.resizer} (Ctrl + R)',
        description: los.resizer_description,
        icon: Symbols.resize,
      ),
      ToolType.clearEditor: Item(
        title: '${los.clear} (F8)',
        description: los.clear_description,
        icon: Symbols.clear_all,
      ),
      ToolType.configEditor: Item(
        title: '${los.config} (F9)',
        description: los.config_description,
        icon: Symbols.settings,
      ),
      ToolType.shortcutMenu: Item(
        title: '${los.shortcut_help} (F2)',
        description: los.shortcut_help_description,
        icon: Symbols.help,
      ),
    });
    return toolItem;
  }

  HashMap<SectionType, Item> _initailizeSection(AppLocalizations los) {
    final sectionItem = HashMap<SectionType, Item>.of({
      SectionType.select: Item(
        title: los.select,
        description: los.section_description,
        icon: Symbols.pan_tool_alt,
      ),
      SectionType.image: Item(
        title: los.island_image,
        description: los.island_image_description,
        icon: Symbols.filter_hdr,
      ),
      SectionType.animation: Item(
        title: los.island_animation,
        description: los.island_animation_description,
        icon: Symbols.filter_drama,
      ),
      SectionType.event: Item(
        title: los.events,
        description: los.events_description,
        icon: Symbols.kid_star,
      ),
    });
    return sectionItem;
  }

  HashMap<ExtensionType, Item> _initailizeExtension(AppLocalizations los) {
    final extensionItem = HashMap<ExtensionType, Item>.of({
      ExtensionType.layer: Item(
        title: los.layer,
        description: los.layer_description,
        icon: Symbols.layers,
      ),
      ExtensionType.history: Item(
        title: los.history,
        description: los.history_description,
        icon: Symbols.history,
      ),
      ExtensionType.setting: Item(
        title: los.map_editor_setting,
        description: los.map_editor_setting_description,
        icon: Symbols.widgets,
      ),
      ExtensionType.palette: Item(
        title: los.palette,
        description: los.palette_description,
        icon: Symbols.palette,
      ),
    });
    return extensionItem;
  }

  HashMap<NavigationType, Item> _initailizeNavigation(AppLocalizations los) {
    final navigationItem = HashMap<NavigationType, Item>.of({
      NavigationType.tool: Item(
        title: los.tool,
        description: los.tool_description,
        icon: Symbols.handyman,
      ),
      NavigationType.item: Item(
        title: los.item,
        description: los.item_description,
        icon: Symbols.package_sharp,
      ),
      NavigationType.option: Item(
        title: los.option,
        description: los.option_description,
        icon: Symbols.settings,
      ),
    });
    return navigationItem;
  }

  HashMap<ActionType, String> _initlActionTypeString(AppLocalizations los) {
    final actionTypeLocalization = HashMap<ActionType, String>.of({
      ActionType.islandChangeID: los.island_change_id,
      ActionType.islandScale: los.island_scale,
      ActionType.islandChangeLayer: los.island_change_layer,
      ActionType.islandChangeParallax: los.island_change_parallax,
      ActionType.islandRotate: los.island_rotate,
      ActionType.islandChangeRotationRate: los.island_change_rotation_rate,
      ActionType.islandArtFlip: los.island_art_flip,
      ActionType.eventChangeID: los.event_change_id,
      ActionType.eventChangeName: los.event_change_name,
      ActionType.eventChangeParent: los.event_change_parent,
      ActionType.eventChangeUnlockedFrom: los.event_change_unlocked_from,
      ActionType.eventChangeVisibleFrom: los.event_change_visible_from,
      ActionType.eventChangeAutoVisible: los.event_change_auto_visible,
      ActionType.eventChangeLevelData: los.event_change_level_data,
      ActionType.eventChangeDisplayText: los.event_change_display_text,
      ActionType.eventChangeNarUnlocked: los.event_change_nar_unlocked,
      ActionType.eventChangeNarCompleted: los.event_change_nar_completed,
      ActionType.eventChangeTutorial: los.event_change_tutorial,
      ActionType.eventChangeTutorialUnlocked:
          los.event_change_tutorial_unlocked,
      ActionType.eventChangeLevelToggle: los.event_change_level_toggle,
      ActionType.eventChangePlantType: los.event_change_plant_type,
      ActionType.eventChangeUpgradeType: los.event_change_upgrade_type,
      ActionType.eventChangeStarCost: los.event_change_star_cost,
      ActionType.eventChangeKeyCost: los.event_change_key_cost,
      ActionType.eventArtFlip: los.event_art_flip,
      ActionType.addItem: los.add_item,
      ActionType.moveItem: los.move_item,
      ActionType.eraseItem: los.erase_item,
      ActionType.pasteItem: los.paste_item,
      ActionType.selectItem: los.select_item,
      ActionType.rectangleSelect: los.rectangle_select,
      ActionType.deSelect: los.deselect,
      ActionType.mapChangeName: los.map_change_name,
      ActionType.mapChangeWorldID: los.map_change_world_id,
      ActionType.mapChangeResID: los.map_change_res_id,
      ActionType.mapChangeBounding: los.map_change_bounding,
      ActionType.newMapEditor: los.new_map_editor,
      ActionType.loadWorldResource: los.load_world_resource,
      ActionType.openWorldMap: los.open_world_map,
      ActionType.createNewLayer: los.create_new_layer,
      ActionType.deleteLayer: los.delete_layer,
      ActionType.mergeDownLayer: los.merge_down_layer,
      ActionType.moveUpLayer: los.move_up_layer,
      ActionType.moveDownLayer: los.move_down_layer,
    });
    return actionTypeLocalization;
  }

  Future<void> load(AppLocalizations los, String resourceLocation) async {
    try {
      final newState = MapEditorConfigurationState(
        status: AppConfigurationStatus.success,
      );
      await Future.delayed(const Duration(milliseconds: 10));
      newState.settingPath = resourceLocation;
      newState.toolItem = _initailizeTool(los);
      newState.sectionItem = _initailizeSection(los);
      newState.extensionItem = _initailizeExtension(los);
      newState.navigationItem = _initailizeNavigation(los);
      newState.actionTypeLocalization = _initlActionTypeString(los);
      newState.configModel = await _loadConfig(
        los,
        '$resourceLocation/config.json',
      );
      newState.editorResource = await _loadEditorResource(
        los,
        resourceLocation,
      );
      newState.gameResource = await _loadGameResource(
        los,
        resourceLocation,
        newState.configModel,
      );
      _setState(newState);
    } catch (ex) {
      _throwErrorState(ex.toString());
    }
    return;
  }
}
