import 'dart:collection';

import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/model/worldmap.dart';
import 'package:sen/cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'package:sen/screen/map_editor/bloc/init_bloc/init_bloc.dart';
import 'package:sen/screen/map_editor/bloc/item/item_bloc.dart';
import 'package:sen/screen/map_editor/bloc/layer/layer_bloc.dart';
import 'package:sen/screen/map_editor/bloc/setting/setting_bloc.dart';
import 'package:sen/screen/map_editor/bloc/stage/stage_bloc.dart';
import 'package:sen/screen/map_editor/models/config.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/service/file_helper.dart';

part 'toolbar_event.dart';
part 'toolbar_state.dart';

class ToolBarBloc extends Bloc<ToolBarEvent, ToolBarState> {
  ToolBarBloc({
    required this.cubit,
    required this.settingBloc,
    required this.initBloc,
    required this.los,
    required this.initialDirectoryCubit,
  }) : super(ToolBarState.initailize()) {
    on<ToolToogled>(_onToggled);
    on<ToolOpenEvent>(_openTool);
    on<ToolSaveEvent>(_saveTool);
    on<ToolConfigEvent>(_configTool);
    on<ToolClearEvent>(_clearTool);
    on<ShortcutMenuEvent>(_shortcutMenu);
    on<ToolConfigSubmitted>(_configToolDone);
    on<ToolClearSubmitted>(_clearToolDone);
  }

  final MapEditorConfigurationCubit cubit;

  final InitialDirectoryCubit initialDirectoryCubit;

  final SettingBloc settingBloc;

  final InitBloc initBloc;

  final AppLocalizations los;

  void _shortcutMenu(
    ShortcutMenuEvent event,
    Emitter<ToolBarState> emit,
  ) {
    initBloc.add(const ShowAlertDialog(
      type: AlertDialogShowType.shortcut,
      enable: true,
    ));
    return;
  }

  void _configTool(
    ToolConfigEvent event,
    Emitter<ToolBarState> emit,
  ) {
    initBloc.add(const ShowAlertDialog(
      type: AlertDialogShowType.config,
      enable: true,
    ));
    return;
  }

  void _configToolDone(
    ToolConfigSubmitted event,
    Emitter<ToolBarState> emit,
  ) {
    final settingPath = '${cubit.state.settingPath}/config.json';
    final config = cubit.state.configModel;
    config.setting.playSingleFrame = settingBloc.state.playSingleFrame;
    config.setting.muteAudio = settingBloc.state.muteAudio;
    config.setting.filterQuality = settingBloc.state.filterQuality;
    final json = FileHelper.readJson(source: settingPath);
    json['setting'] = ConfigSetting.toJson(config.setting);
    FileHelper.writeJson(source: settingPath, data: json);
    if (!settingBloc.state.muteAudio) {
      cubit.state.editorResource.switchResourceSound.resume();
    }
    initBloc.add(ShowSnackBarEvent(text: los.config_saved));
    return;
  }

  void _clearTool(
    ToolClearEvent event,
    Emitter<ToolBarState> emit,
  ) {
    initBloc.add(const ShowAlertDialog(
      type: AlertDialogShowType.clear,
      enable: true,
    ));
    return;
  }

  void _clearToolDone(
    ToolClearSubmitted event,
    Emitter<ToolBarState> emit,
  ) {
    event.stageBloc.add(ClearWorldEvent(
      itemUpdate: event.itemUpdate,
      layerBloc: event.layerBloc,
    ));
  }

  Future<void> _saveTool(
    ToolSaveEvent event,
    Emitter<ToolBarState> emit,
  ) async {
    var path = await FileHelper.saveFile(
      initialDirectory: initialDirectoryCubit.state.initialDirectory,
      suggestedName: 'worldmap.json',
    );
    if (path != null) {
      if (RegExp(r'\.json$', caseSensitive: false).hasMatch(path)) {
        path += '.json';
      }
      final state = event.stageBloc.state;
      final worldMap = WorldMap(list: [
        WorldData(
            worldName: state.worldName,
            worldID: state.worldId,
            resGroupID: state.resGroupId,
            boundingRect: state.boundingRect,
            pieces: state.pieces.values.toList(),
            events: state.events.values.toList(),
            creationTime: DateTime.now().millisecondsSinceEpoch ~/ 1000)
      ]);
      FileHelper.writeJson(source: path, data: WorldMap.toJson(worldMap));
      if (!settingBloc.state.muteAudio) {
        cubit.state.editorResource.switchResourceSound.resume();
      }
      initBloc.add(
        ShowSnackBarEvent(text: los.worldmap_saved),
      );
      //event.autosaveBloc.add(const CleanAutosaveEvent());
    }
  }

  Future<void> _openTool(
    ToolOpenEvent event,
    Emitter<ToolBarState> emit,
  ) async {
    final path = await FileHelper.uploadFile(
      initialDirectory: initialDirectoryCubit.state.initialDirectory,
    );
    if (path == null) {
      return;
    }
    try {
      final worldMap = WorldMap.fromJson(FileHelper.readJson(source: path));
      event.stageBloc.add(LoadWorldEvent(
          worldData: worldMap.list.first,
          itemBloc: event.itemBloc,
          layerBloc: event.layerBloc,
          stageBloc: event.stageBloc));
    } catch (ex) {
      initBloc.add(
        ShowSnackBarEvent(
          text: los.failed_to_load_worldmap,
        ),
      );
    }
  }

  void _onToggled(
    ToolToogled event,
    Emitter<ToolBarState> emit,
  ) {
    final toolStatus = state.toolStatus[event.type];
    final toolBarState = state.copyWith();
    toolBarState.toolStatus[event.type] = event.enabled ?? !toolStatus!;
    emit(toolBarState);
  }

  bool onEnabled(ToolType type) {
    final toolStatus = state.toolStatus[type];
    assert(toolStatus != null, 'failed get tool stats by tool type $type');
    return toolStatus!;
  }
}
