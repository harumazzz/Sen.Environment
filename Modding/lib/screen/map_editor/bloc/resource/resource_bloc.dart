import 'dart:async';
import 'dart:collection';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:matrix4_transform/matrix4_transform.dart';
import 'package:sen/cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'package:sen/screen/map_editor/bloc/init_bloc/init_bloc.dart';
import 'package:sen/screen/map_editor/bloc/setting/setting_bloc.dart';
import 'package:sen/screen/map_editor/include/visual_animation.dart';
import 'package:sen/screen/map_editor/include/visual_image.dart';
import 'package:sen/screen/map_editor/models/config.dart';
import 'package:sen/screen/map_editor/models/event_node.dart';
import 'package:sen/screen/map_editor/models/game_resource.dart';
import 'package:sen/screen/map_editor/widgets/animation_widget.dart';
import 'package:sen/service/file_helper.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:path/path.dart' as path;

part 'resource_event.dart';
part 'resource_state.dart';

class ResourceBloc extends Bloc<ResourceEvent, ResourceState> {
  ResourceBloc({
    required this.cubit,
    required this.settingBloc,
    required this.initBloc,
    required this.los,
  }) : super(ResourceState()) {
    on<LoadResourceByWorldName>(_loadResourceByWorld);
    on<ResourceLoading>(_resourceLoading);
    on<ResourceClear>(_clear);
  }

  final MapEditorConfigurationCubit cubit;

  final SettingBloc settingBloc;

  final InitBloc initBloc;

  final AppLocalizations los;

  void _clear(ResourceClear event, Emitter<ResourceState> emit) {
    emit(ResourceState());
  }

  void _resourceLoading(ResourceLoading event, Emitter<ResourceState> emit) {
    emit(ResourceState(status: ResourceStateStatus.loading));
  }

  void _finished(bool notify, Function itemUpdate) {
    itemUpdate();
    if (notify) {
      if (!settingBloc.state.muteAudio) {
        cubit.state.editorResource.switchResourceSound.resume();
      }
      initBloc.add(
        ShowSnackBarEvent(
          text: los.world_resources_changed,
        ),
      );
    }
  }

  Widget _createAnimationItemShop(VisualAnimation visual,
      {required Iterable<String> label}) {
    return FittedBox(
        fit: BoxFit.contain,
        child: UnconstrainedBox(
            child: SizedOverflowBox(
                alignment: AlignmentDirectional.topStart,
                size: visual.visualSize,
                child: AnimationWidget(
                  visual: visual,
                  labelPlay: label,
                  borderWidth: 0.0,
                ))));
  }

  Iterable<String> _getPlayLabel(EventNodeType type, {bool locked = false}) {
    if (eventAnimationLabel.containsKey(type)) {
      return locked
          ? eventAnimationLabel[type]!.$1
          : eventAnimationLabel[type]!.$2;
    }
    return const ['main'];
  }

  Future<void> _loadEventResource(ResourceState state,
      MapEditorConfigurationCubit cubit, FilterQuality filterQuality) async {
    final gameResource = cubit.state.gameResource;
    final bossVisual =
        state.resourceAnimation[ResourceAnimationType.zombossNode];
    final bossItem = Transform(
        alignment: Alignment.center,
        transform: Matrix4Transform()
            .scale(bossVisual != null ? 0.5 : 1.8)
            .translate(y: bossVisual != null ? 10 : 0)
            .m,
        child: _createAnimationItemShop(
            bossVisual ??
                gameResource.commonAnimation[
                    AnimationCommonType.missingArtPieceAnimation]!,
            label: _getPlayLabel(EventNodeType.boss)));
    final keyGateVisual =
        state.resourceAnimation[ResourceAnimationType.keyGate];
    final keyGateItem = Transform(
        alignment: Alignment.center,
        transform: Matrix4Transform()
            .scale(keyGateVisual != null ? 1.3 : 1.8)
            .translate(
                x: keyGateVisual != null ? -5 : 0,
                y: keyGateVisual != null ? 10 : 0)
            .m,
        child: _createAnimationItemShop(
            keyGateVisual ??
                gameResource.commonAnimation[
                    AnimationCommonType.missingArtPieceAnimation]!,
            label: _getPlayLabel(EventNodeType.keygate)));
    state.eventShop.addAll({
      EventNodeType.normal: Transform.scale(
          scale: 2,
          child: _createAnimationItemShop(
              gameResource.commonAnimation[AnimationCommonType.levelNode]!,
              label: _getPlayLabel(EventNodeType.normal))),
      EventNodeType.minigame: Transform.scale(
          scale: 2,
          child: _createAnimationItemShop(
              gameResource
                  .commonAnimation[AnimationCommonType.levelNodeMinigame]!,
              label: _getPlayLabel(EventNodeType.minigame))),
      EventNodeType.miniboss: Transform.scale(
          scale: 2,
          child: _createAnimationItemShop(
              gameResource
                  .commonAnimation[AnimationCommonType.levelNodeGargantuar]!,
              label: _getPlayLabel(EventNodeType.miniboss))),
      EventNodeType.nonfinalboss: Transform.scale(
          scale: 2,
          child: _createAnimationItemShop(
              gameResource
                  .commonAnimation[AnimationCommonType.levelNodeGargantuar]!,
              label: _getPlayLabel(EventNodeType.nonfinalboss))),
      EventNodeType.boss: bossItem,
      EventNodeType.danger: _createAnimationItemShop(
          state.resourceAnimation[ResourceAnimationType.dangerNode] ??
              gameResource.commonAnimation[
                  AnimationCommonType.missingArtPieceAnimation]!,
          label: _getPlayLabel(EventNodeType.danger)),
      EventNodeType.giftbox: Transform.scale(
          scale: 2,
          child: Transform.translate(
              offset: const Offset(0, 10),
              child: _createAnimationItemShop(
                  gameResource.commonAnimation[AnimationCommonType.giftBox]!,
                  label: _getPlayLabel(EventNodeType.giftbox, locked: true)))),
      EventNodeType.pinata: Transform.scale(
          scale: 1.3,
          child: RawImage(
            image: state.resourceImage[ResourceImageType.pinataSpine] ??
                gameResource.commonImage[ImageCommonType.freePinata],
            filterQuality: filterQuality,
          )),
      EventNodeType.plant: Stack(
        fit: StackFit.passthrough,
        children: [
          Transform.scale(
              scale: 0.9,
              child: RawImage(
                  image:
                      gameResource.commonImage[ImageCommonType.readySeedBank])),
          Transform(
            transform: Matrix4Transform().scale(0.5).translate(x: 10, y: 22).m,
            child: RawImage(
                image: gameResource.commonImage[ImageCommonType.readyPacket]),
          ),
        ],
      ),
      EventNodeType.upgrade: Transform.scale(
          scale: 0.9,
          child: RawImage(
              image: gameResource.upgrade.values.firstOrNull ??
                  gameResource.commonImage[ImageCommonType.missingArtPiece])),
      EventNodeType.stargate: Transform.scale(
          scale: 1.5,
          child: _createAnimationItemShop(
              gameResource.commonAnimation[AnimationCommonType.stargate]!,
              label: _getPlayLabel(EventNodeType.stargate))),
      EventNodeType.keygate: keyGateItem,
      EventNodeType.pathNode: RawImage(
        image: gameResource.commonImage[ImageCommonType.pathNode],
        filterQuality: filterQuality,
      ),
      EventNodeType.doodad: Transform.scale(
          scale: 0.5,
          child: RawImage(
            image: gameResource.commonImage[ImageCommonType.doodad],
            filterQuality: filterQuality,
          )),
    });
  }

  void _loadEventNodeName(ResourceState state) {
    state.eventNodeName.addAll({
      //TODO: add locale
      EventNodeType.pinata: 'Pinata',
      EventNodeType.plant: 'Plant',
      EventNodeType.giftbox: 'Gift Box',
      EventNodeType.upgrade: 'Upgrade',
      EventNodeType.normal: 'Normal Level',
      EventNodeType.minigame: 'Minigame Level',
      EventNodeType.miniboss: 'Miniboss Level',
      EventNodeType.nonfinalboss: 'Nonfinal Boss',
      EventNodeType.boss: 'Boss Level',
      EventNodeType.danger: 'Danger',
      EventNodeType.stargate: 'Star Gate',
      EventNodeType.keygate: 'Key Gate',
      EventNodeType.pathNode: 'Path Node',
      EventNodeType.doodad: 'Doodad',
    });
  }

  Future<void> _loadResourceByWorld(
      LoadResourceByWorldName event, Emitter<ResourceState> emit) async {
    final newState = ResourceState(status: ResourceStateStatus.finished);
    final loadPath = '${cubit.state.settingPath}/worldmap/${event.worldName}';
    /*
    final animationMissing = cubit.state.gameResource
        .commonAnimation[AnimationCommonType.missingArtPieceAnimation]!;
    */
    final filterQuality = settingBloc.state.filterQuality;
    if (event.worldName == 'none') {
      _finished(event.notify, event.itemUpdate);
      emit(newState);
      return;
    }
    if (!FileHelper.isDirectory(loadPath)) {
      initBloc.add(ShowSnackBarEvent(
          text: '${los.cannot_find_world}: ${event.worldName}'));
      emit(newState);
      return;
    }
    _loadEventNodeName(newState);
    for (final e
        in FileHelper.readDirectory(source: loadPath, recursive: false)) {
      final baseName = path.basenameWithoutExtension(e).toLowerCase();
      if (baseName.startsWith('island')) {
        final image = await cubit.loadVisualImage(e);
        final id = int.tryParse(baseName.substring(6));
        if (image != null && id != null) {
          newState.islandImage[id] = image;
        }
      } else if (baseName.startsWith('anim')) {
        final animation =
            await cubit.loadVisualAnimation(e, filterQuality: filterQuality);
        final id = int.tryParse(baseName.substring(4));
        if (animation != null && id != null) {
          newState.islandAnimation[id] = animation;
          newState.rasterizedInAnimation[id] =
              event.animationDetails[baseName]?.usesRasterizedImagesInAnim ??
                  false;
        }
      } else if (baseName.startsWith('danger_node')) {
        final animation =
            await cubit.loadVisualAnimation(e, filterQuality: filterQuality);
        newState.resourceAnimation[ResourceAnimationType.dangerNode] =
            animation;
      } else if (baseName.startsWith('zomboss_node')) {
        final animation =
            await cubit.loadVisualAnimation(e, filterQuality: filterQuality);
        newState.resourceAnimation[ResourceAnimationType.zombossNode] =
            animation;
      } else if (baseName.startsWith('gate')) {
        final animation =
            await cubit.loadVisualAnimation(e, filterQuality: filterQuality);
        newState.resourceAnimation[ResourceAnimationType.keyGate] = animation;
      } else if (baseName.startsWith('danger_level')) {
        final image = await cubit.loadVisualImage(e);
        newState.resourceImage[ResourceImageType.dangerLevel] = image;
      }
    }
    final pinataPath = '${cubit.state.settingPath}/pinata';
    newState.resourceImage[ResourceImageType.pinataSpine] = await cubit
        .loadVisualImage('$pinataPath/pinata_${event.worldName}_spine.png');
    newState.resourceImage[ResourceImageType.pinataSpineOpen] =
        await cubit.loadVisualImage(
            '$pinataPath/pinatas_dust_spine_${event.worldName}.png');
    await _loadEventResource(newState, cubit, filterQuality);
    emit(newState);
    _finished(event.notify, event.itemUpdate);
  }
}
