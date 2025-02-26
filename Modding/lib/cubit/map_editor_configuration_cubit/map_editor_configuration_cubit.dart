import 'dart:async';
import 'dart:collection';
import 'dart:io';

import 'package:audioplayers/audioplayers.dart';
import 'package:custom_mouse_cursor/custom_mouse_cursor.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/model/animation.dart';
import 'package:sen/model/item.dart';
import 'package:sen/screen/map_editor/bloc/toolbar/toolbar_bloc.dart';
import 'package:sen/screen/map_editor/bloc/section/section_bloc.dart';
import 'package:sen/screen/map_editor/include/visual_animation.dart';
import 'package:sen/screen/map_editor/include/visual_image.dart';
import 'package:sen/screen/map_editor/models/action_service.dart';
import 'package:sen/screen/map_editor/models/config.dart';
import 'package:sen/screen/map_editor/models/editor_resource.dart';
import 'package:sen/screen/map_editor/models/event_node.dart';
import 'package:sen/screen/map_editor/models/game_resource.dart';
import 'package:sen/service/file_helper.dart';
import 'package:sen/i18n/app_localizations.dart';

part 'map_editor_configuration_state.dart';

class MapEditorConfigurationCubit extends Cubit<MapEditorConfigurationState> {
  MapEditorConfigurationCubit() : super(MapEditorConfigurationState());

  void initializeState() => emit(MapEditorConfigurationState());

  void _setState(MapEditorConfigurationState state) => emit(state);

  void _throwErrorState(String ex) {
    emit(MapEditorConfigurationState(status: AppConfigurationStatus.failed, errorSnapShot: ex));
  }

  ConfigModel _loadConfig(AppLocalizations los, String path) {
    return ConfigModel.fromJson(FileHelper.readJson(source: path));
  }

  bool get isDesktopPlatform {
    return Platform.isWindows || Platform.isLinux || Platform.isMacOS;
  }

  Future<EditorResource> _loadEditorResource(
    AppLocalizations los,
    String path,
  ) async {
    final eraseCursor =
        isDesktopPlatform ? await CustomMouseCursor.icon(Symbols.ink_eraser, size: 28, color: Colors.white) : null;
    final panCursor =
        isDesktopPlatform ? await CustomMouseCursor.icon(Symbols.pan_tool, size: 28, color: Colors.white) : null;
    final multiSelectCursor =
        isDesktopPlatform ? await CustomMouseCursor.icon(Symbols.pan_tool_alt, size: 28, color: Colors.white) : null;
    final pickItemSound = AudioPlayer();
    try {
      pickItemSound.setSourceBytes(FileHelper.readBuffer(source: '$path/sound/grab2.mp3'));
      pickItemSound.setReleaseMode(ReleaseMode.stop);
    }
    // ignore: empty_catches
    catch (ex) {}
    final removeItemSound = AudioPlayer();
    try {
      removeItemSound.setSourceBytes(FileHelper.readBuffer(source: '$path/sound/shell_hit.mp3'));
      removeItemSound.setReleaseMode(ReleaseMode.stop);
    }
    // ignore: empty_catches
    catch (ex) {}
    final setItemSound = AudioPlayer();
    try {
      setItemSound.setSourceBytes(FileHelper.readBuffer(source: '$path/sound/smb2_throw.mp3'));
      setItemSound.setReleaseMode(ReleaseMode.stop);
    }
    // ignore: empty_catches
    catch (ex) {}
    final mapLoadedSound = AudioPlayer();
    try {
      mapLoadedSound.setSourceBytes(FileHelper.readBuffer(source: '$path/sound/level_select.mp3'));
      mapLoadedSound.setReleaseMode(ReleaseMode.stop);
    }
    // ignore: empty_catches
    catch (ex) {}
    final clearMapSound = AudioPlayer();
    try {
      clearMapSound.setSourceBytes(FileHelper.readBuffer(source: '$path/sound/smash.mp3'));
      clearMapSound.setReleaseMode(ReleaseMode.stop);
    }
    // ignore: empty_catches
    catch (ex) {}
    final switchResourceSound = AudioPlayer();
    try {
      switchResourceSound.setSourceBytes(FileHelper.readBuffer(source: '$path/sound/has_item.mp3'));
      switchResourceSound.setReleaseMode(ReleaseMode.stop);
    }
    // ignore: empty_catches
    catch (ex) {}
    final senLogo = Image.asset(
      'assets/images/logo.png',
      opacity: const AlwaysStoppedAnimation(.5),
    );

    return EditorResource(
        senLogo: senLogo,
        eraseCursor: eraseCursor,
        panCursor: panCursor,
        multiSelectCursor: multiSelectCursor,
        pickItemSound: pickItemSound,
        removeItemSound: removeItemSound,
        setItemSound: setItemSound,
        mapLoadedSound: mapLoadedSound,
        clearMapSound: clearMapSound,
        switchResourceSound: switchResourceSound);
  }

  Future<VisualImage?> loadVisualImage(String path) async {
    try {
      final data = FileHelper.readBuffer(source: path);
      final completer = Completer<VisualImage>();
      final listener = ImageStreamListener((info, _) {
        completer.complete(info.image);
      });
      final stream = MemoryImage(data).resolve(const ImageConfiguration())..addListener(listener);
      final image = await completer.future;
      stream.removeListener(listener);
      return image;
    } catch (ex) {
      return null;
    }
  }

  Future<VisualAnimation?> loadVisualAnimation(String path, {FilterQuality? filterQuality}) async {
    try {
      final animationPath = '$path/animation.pam.json';
      final mediaPath = '$path/media';
      final visual = await VisualAnimation.create(animationPath, mediaPath, filterQuality: filterQuality);
      return visual;
    } catch (ex) {
      return null;
    }
  }

  Future<VisualAnimation?> loadPlantVisualAnimation(String path, String plantType, bool enableCostume,
      {FilterQuality? filterQuality}) async {
    try {
      final animationPath = '$path/animation.pam.json';
      final mediaPath = '$path/media';
      final animation = SexyAnimation.fromJson(FileHelper.readJson(source: animationPath));
      final spriteDisable = getCostumeSpriteDisable(plantType.toLowerCase(), animation.sprite.map((e) => e.name),
          enableCostume: enableCostume);
      final visual = await VisualAnimation.createByAnimation(animation, mediaPath,
          filterQuality: filterQuality, spriteDisable: spriteDisable);
      return visual;
    } catch (ex) {
      return null;
    }
  }

  Future<GameResource> _loadGameResource(AppLocalizations los, String path, ConfigModel config) async {
    final filterQuality = config.setting.filterQuality;
    final commonImage = HashMap<ImageCommonType, VisualImage>();
    //
    final missingArtPiece = await loadVisualImage('$path/alwaysloaded/missing_artpiece.png');
    if (missingArtPiece == null) {
      throw Exception(los.cannot_load_missing_artpiece);
    }
    commonImage[ImageCommonType.missingArtPiece] = missingArtPiece;
    final readySeedBank = await loadVisualImage('$path/packet/ready.png');
    if (readySeedBank == null) {
      throw Exception(los.cannot_load_ready_seedbank);
    }
    commonImage[ImageCommonType.readySeedBank] = readySeedBank;
    final readyPacket = await loadVisualImage('$path/packet/sunflower.png');
    if (readyPacket == null) {
      throw Exception(los.cannot_load_ready_packet);
    }
    commonImage[ImageCommonType.readyPacket] = readyPacket;
    final spaceSpiral = await loadVisualImage('$path/alwaysloaded/space_spiral.png');
    if (spaceSpiral == null) {
      throw Exception(los.cannot_load_space_spiral);
    }
    commonImage[ImageCommonType.spaceSpiral] = spaceSpiral;
    final spaceDust = await loadVisualImage('$path/alwaysloaded/space_dust.png');
    if (spaceDust == null) {
      throw Exception(los.cannot_load_space_dust);
    }
    commonImage[ImageCommonType.spaceDust] = spaceDust;
    final freePinata = await loadVisualImage('$path/pinata/pinata_free_spine.png');
    if (freePinata == null) {
      throw Exception(los.cannot_load_free_pinata);
    }
    commonImage[ImageCommonType.freePinata] = freePinata;
    final freePinataOpen = await loadVisualImage('$path/pinata/pinatas_dust_spine_free.png');
    if (freePinataOpen == null) {
      throw Exception(los.cannot_load_free_pinata_dust);
    }
    commonImage[ImageCommonType.freePinataOpen] = freePinataOpen;
    final buttonHudBackNormal = await loadVisualImage('$path/common/buttons_hud_back_normal.png');
    if (buttonHudBackNormal == null) {
      throw Exception(los.cannot_load_buttons_hud_back_normal);
    }
    commonImage[ImageCommonType.buttonHudBackNormal] = buttonHudBackNormal;
    final buttonHudBackSelected = await loadVisualImage('$path/common/buttons_hud_back_selected.png');
    if (buttonHudBackSelected == null) {
      throw Exception(los.cannot_load_buttons_hud_back_selected);
    }
    commonImage[ImageCommonType.buttonHudBackSelected] = buttonHudBackSelected;
    final keygateFlag = await loadVisualImage('$path/common/keygate_flag.png');
    if (keygateFlag == null) {
      throw Exception(los.cannot_load_key_gate_flag);
    }
    commonImage[ImageCommonType.keygateFlag] = keygateFlag;
    final infoIcon = await loadVisualImage('$path/common/info_icon.png');
    if (infoIcon == null) {
      throw Exception(los.cannot_load_info_icon);
    }
    commonImage[ImageCommonType.infoIcon] = infoIcon;

    final sprout = await loadVisualImage('$path/common/sprout.png');
    if (sprout == null) {
      throw Exception(los.cannot_load_sprout);
    }
    commonImage[ImageCommonType.sprout] = sprout;
    final doodad = await loadVisualImage('$path/common/doodad1.png');
    if (doodad == null) {
      throw Exception(los.cannot_load_doodad);
    }
    commonImage[ImageCommonType.doodad] = doodad;
    final pathNode = await loadVisualImage('$path/common/grass_light.png');
    if (pathNode == null) {
      throw Exception(los.cannot_load_pathNode);
    }
    commonImage[ImageCommonType.pathNode] = pathNode;
    //-----------------
    final commonAnimation = HashMap<AnimationCommonType, VisualAnimation>();
    //
    final giftBox = await loadVisualAnimation('$path/common/giftbox_world_map', filterQuality: filterQuality);
    if (giftBox == null) {
      throw Exception(los.cannot_load_giftbox_world_map);
    }
    commonAnimation[AnimationCommonType.giftBox] = giftBox;
    final levelNode = await loadVisualAnimation('$path/common/level_node', filterQuality: filterQuality);
    if (levelNode == null) {
      throw Exception(los.cannot_load_level_node);
    }
    commonAnimation[AnimationCommonType.levelNode] = levelNode;
    final levelNodeGargantuar =
        await loadVisualAnimation('$path/common/level_node_gargantuar', filterQuality: filterQuality);
    if (levelNodeGargantuar == null) {
      throw Exception(los.cannot_load_level_node_gargantuar);
    }
    commonAnimation[AnimationCommonType.levelNodeGargantuar] = levelNodeGargantuar;
    final levelNodeMinigame =
        await loadVisualAnimation('$path/common/level_node_minigame', filterQuality: filterQuality);
    if (levelNodeMinigame == null) {
      throw Exception(los.cannot_load_level_node_minigame);
    }
    commonAnimation[AnimationCommonType.levelNodeMinigame] = levelNodeMinigame;
    final mapPath = await loadVisualAnimation('$path/common/map_path', filterQuality: filterQuality);
    if (mapPath == null) {
      throw Exception(los.cannot_load_map_path);
    }
    commonAnimation[AnimationCommonType.mapPath] = mapPath;
    final yetiIcon = await loadVisualAnimation('$path/common/yeti_icon', filterQuality: filterQuality);
    if (yetiIcon == null) {
      throw Exception(los.cannot_load_yeti_icon);
    }
    commonAnimation[AnimationCommonType.yetiIcon] = yetiIcon;
    final zombossNodeHologram =
        await loadVisualAnimation('$path/common/zomboss_node_hologram', filterQuality: filterQuality);
    if (zombossNodeHologram == null) {
      throw Exception(los.cannot_load_zomboss_node_hologram);
    }
    commonAnimation[AnimationCommonType.zombossNodeHologram] = zombossNodeHologram;
    final missingArtPieceAnimation =
        await loadVisualAnimation('$path/alwaysloaded/missing_artpiece', filterQuality: filterQuality);
    if (missingArtPieceAnimation == null) {
      throw Exception(los.cannot_load_map_path);
    }

    final readyPlant = await loadPlantVisualAnimation(
      '$path/plant/sunflower',
      filterQuality: filterQuality,
      'ready',
      config.setting.plantCostume,
    );
    if (readyPlant == null) {
      throw Exception(los.cannot_load_ready_plant);
    }
    commonAnimation[AnimationCommonType.readyPlant] = readyPlant;
    commonAnimation[AnimationCommonType.missingArtPieceAnimation] = missingArtPieceAnimation;
    final stargate = await loadVisualAnimation('$path/common/stargate', filterQuality: filterQuality);
    if (stargate == null) {
      throw Exception(los.cannot_load_star_gate);
    }
    commonAnimation[AnimationCommonType.stargate] = stargate;
    final sodRoll = await loadVisualAnimation('$path/common/sod_roll', filterQuality: filterQuality);
    if (sodRoll == null) {
      throw Exception(los.cannot_load_sod_roll);
    }
    commonAnimation[AnimationCommonType.sodRoll] = sodRoll;
    final collectedUpgradeEffect =
        await loadVisualAnimation('$path/common/collected_upgrade_effect', filterQuality: filterQuality);
    if (collectedUpgradeEffect == null) {
      throw Exception(los.cannot_load_collected_upgrade_effect);
    }
    commonAnimation[AnimationCommonType.collectedUpgradeEffect] = collectedUpgradeEffect;
    //-----------------
    final uiUniverse = HashMap<String, VisualImage>();
    //
    for (final mapName in config.resource.worldmap.keys) {
      uiUniverse[mapName] = await loadVisualImage('$path/alwaysloaded/ui_universe/$mapName.png') ?? missingArtPiece;
    }
    //-----------------
    final seedBank = HashMap<String, VisualImage?>();
    final packet = HashMap<String, VisualImage?>();
    final plant = HashMap<String, VisualAnimation?>();
    //
    for (final plantName in config.resource.plant.keys) {
      final seedBankName = config.resource.plant[plantName];
      if (!seedBank.containsKey(seedBankName)) {
        seedBank[seedBankName] = await loadVisualImage('$path/packet/$seedBankName.png');
      }
      packet[plantName] = await loadVisualImage('$path/packet/$plantName.png');
      // plant[plantName] = await loadVisualAnimation(
      //   '$path/plant/$plantName',
      //   filterQuality: filterQuality,
      //);
    }
    //
    final upgrade = HashMap<String, VisualImage?>();
    //
    for (final upgradeName in config.resource.upgrade.keys) {
      final src = config.resource.upgrade[upgradeName];
      upgrade[upgradeName] = await loadVisualImage('$path/upgrade/upgrade_$src.png');
    }
    //-----------------
    return GameResource(
        commonImage: commonImage,
        commonAnimation: commonAnimation,
        uiUniverse: uiUniverse,
        seedBank: seedBank,
        packet: packet,
        plant: plant,
        upgrade: upgrade);
  }

  HashMap<ToolType, Item> _initailizeTool(AppLocalizations los) {
    final toolItem = HashMap<ToolType, Item>.from({
      ToolType.openFile: Item(
        title: '${los.open} (Ctrl + O)',
        description: los.open_description,
        icon: const Icon(Symbols.file_open),
        isEnabled: true,
      ),
      ToolType.saveFile: Item(
        title: '${los.save} (Ctrl + S)',
        description: los.save_description,
        icon: const Icon(Symbols.save),
        isEnabled: true,
      ),
      ToolType.rectangleTool: Item(
        title: '${los.rectangle_tool} (Ctrl Left)',
        description: los.select_multiple_items_by_rectangle,
        icon: const Icon(Symbols.gesture_select),
        isEnabled: true,
      ),
      ToolType.eraseTool: Item(
        title: '${los.eraser} (E)',
        description: los.eraser_description,
        icon: const Icon(Symbols.ink_eraser),
        isEnabled: true,
      ),
      ToolType.panTool: Item(
        title: '${los.pan_tool} (Space)',
        description: los.pan_tool_description,
        icon: const Icon(Symbols.pan_tool),
        isEnabled: true,
      ),
      ToolType.resizeTool: Item(
        title: '${los.resizer} (Ctrl + R)',
        description: los.resizer_description,
        icon: const Icon(Symbols.resize),
        isEnabled: true,
      ),
      ToolType.clearEditor: Item(
        title: '${los.clear} (F8)',
        description: los.clear_description,
        icon: const Icon(Symbols.clear_all),
        isEnabled: true,
      ),
      ToolType.configEditor: Item(
        title: '${los.config} (F9)',
        description: los.config_description,
        icon: const Icon(Symbols.settings),
        isEnabled: true,
      ),
      ToolType.shortcutMenu: Item(
        title: '${los.shortcut_help} (F2)',
        description: los.shortcut_help_description,
        icon: const Icon(Symbols.help),
        isEnabled: true,
      ),
    });
    return toolItem;
  }

  HashMap<SectionType, Item> _initailizeSection(AppLocalizations los) {
    final sectionItem = HashMap<SectionType, Item>.from({
      SectionType.select: Item(
        title: los.select,
        description: los.section_description,
        icon: const Icon(Symbols.pan_tool_alt),
        isEnabled: true,
      ),
      SectionType.image: Item(
        title: los.island_image,
        description: los.island_image_description,
        icon: const Icon(Symbols.filter_hdr),
        isEnabled: true,
      ),
      SectionType.animation: Item(
        title: los.island_animation,
        description: los.island_animation_description,
        icon: const Icon(Symbols.filter_drama),
        isEnabled: true,
      ),
      SectionType.event: Item(
        title: los.events,
        description: los.events_description,
        icon: const Icon(Symbols.kid_star),
        isEnabled: true,
      )
    });
    return sectionItem;
  }

  HashMap<ExtensionType, Item> _initailizeExtension(AppLocalizations los) {
    final extensionItem = HashMap<ExtensionType, Item>.from({
      ExtensionType.layer: Item(
        title: los.layer,
        description: los.layer_description,
        icon: const Icon(Symbols.layers),
        isEnabled: true,
      ),
      ExtensionType.history: Item(
        title: los.history,
        description: los.history_description,
        icon: const Icon(Symbols.history),
        isEnabled: true,
      ),
      ExtensionType.setting: Item(
        title: los.map_editor_setting,
        description: los.map_editor_setting_description,
        icon: const Icon(Symbols.widgets),
        isEnabled: true,
      ),
      ExtensionType.palette: Item(
        title: los.palette,
        description: los.palette_description,
        icon: const Icon(Symbols.palette),
        isEnabled: true,
      )
    });
    return extensionItem;
  }

  HashMap<NavigationType, Item> _initailizeNavigation(AppLocalizations los) {
    final navigationItem = HashMap<NavigationType, Item>.from({
      NavigationType.tool: Item( 
        title: "Tool", //TODO:
        description: "description",
        icon: const Icon(Symbols.handyman),
        isEnabled: true,
      ),
      NavigationType.item: Item(
        title: "Item", //TODO:
        description: "description",
        icon: const Icon(Symbols.package_sharp),
        isEnabled: true,
      ),
      NavigationType.option: Item(
        title: "Option", //TODO:
        description: "description", 
        icon: const Icon(Symbols.settings),
        isEnabled: true,
      ),
    });
    return navigationItem;
  }

  HashMap<ActionType, String> _initlActionTypeString(AppLocalizations los) {
    final actionTypeLocalization = HashMap<ActionType, String>.from({
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
      ActionType.eventChangeTutorialUnlocked: los.event_change_tutorial_unlocked,
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
      final newState = MapEditorConfigurationState(status: AppConfigurationStatus.success);
      await Future.delayed(const Duration(milliseconds: 10));
      newState.settingPath = resourceLocation;
      newState.toolItem = _initailizeTool(los);
      newState.sectionItem = _initailizeSection(los);
      newState.extensionItem = _initailizeExtension(los);
      newState.navigationItem = _initailizeNavigation(los);
      newState.actionTypeLocalization = _initlActionTypeString(los);
      newState.configModel = _loadConfig(los, '$resourceLocation/config.json');
      newState.editorResource = await _loadEditorResource(los, resourceLocation);
      newState.gameResource = await _loadGameResource(los, resourceLocation, newState.configModel);
      _setState(newState);
    } catch (ex) {
      _throwErrorState(ex.toString());
    }
    return;
  }
}
