import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../include/capture_manager.dart';
import '../../models/action_service.dart';
import '../../models/action_model.dart';

part 'history_event.dart';
part 'history_state.dart';

class HistoryBloc extends Bloc<HistoryEvent, HistoryState> {
  HistoryBloc() : super(HistoryState.initailize()) {
    on<InitailizeCapture>(_initialize);
    on<CaptureState>(_capture);
    on<RedoEvent>(_redo);
    on<UndoEvent>(_undo);
    on<ClearHistoryEvent>(_clear);
    on<UpdateIndexEvent>(_update);
  }

  IconData getIconByType(ActionType type) {
    switch (type) {
      case ActionType.islandChangeID:
      case ActionType.islandScale:
      case ActionType.islandChangeLayer:
      case ActionType.islandChangeParallax:
      case ActionType.islandRotate:
      case ActionType.islandChangeRotationRate:
      case ActionType.islandArtFlip:
        return Symbols.landscape;
      case ActionType.eventChangeID:
      case ActionType.eventChangeName:
      case ActionType.eventChangeParent:
      case ActionType.eventChangeUnlockedFrom:
      case ActionType.eventChangeVisibleFrom:
      case ActionType.eventChangeAutoVisible:
      case ActionType.eventChangeLevelData:
      case ActionType.eventChangeDisplayText:
      case ActionType.eventChangeNarUnlocked:
      case ActionType.eventChangeNarCompleted:
      case ActionType.eventChangeTutorial:
      case ActionType.eventChangeTutorialUnlocked:
      case ActionType.eventChangeLevelToggle:
      case ActionType.eventChangePlantType:
      case ActionType.eventChangeUpgradeType:
      case ActionType.eventChangeStarCost:
      case ActionType.eventChangeKeyCost:
      case ActionType.eventArtFlip:
        return Symbols.kid_star;
      case ActionType.addItem:
        return Symbols.add;
      case ActionType.moveItem:
        return Symbols.drag_pan;
      case ActionType.eraseItem:
        return Symbols.ink_eraser;
      case ActionType.pasteItem:
        return Symbols.content_paste;
      case ActionType.mapChangeName:
      case ActionType.mapChangeWorldID:
      case ActionType.mapChangeResID:
        return Symbols.map;
      case ActionType.mapChangeBounding:
        return Symbols.resize;
      case ActionType.selectItem:
        return Symbols.photo_size_select_large;
      case ActionType.rectangleSelect:
        return Symbols.select;
      case ActionType.deSelect:
        return Symbols.remove_selection;
      case ActionType.newMapEditor:
      case ActionType.loadWorldResource:
        return Symbols.new_window;
      case ActionType.openWorldMap:
        return Symbols.file_open;
      case ActionType.createNewLayer:
        return Symbols.create_new_folder;
      case ActionType.deleteLayer:
        return Symbols.delete_sweep;
      case ActionType.mergeDownLayer:
        return Symbols.text_select_move_down;
      case ActionType.moveUpLayer:
        return Symbols.move_selection_up;
      case ActionType.moveDownLayer:
        return Symbols.move_selection_down;
    }
  }

  void _update(UpdateIndexEvent event, Emitter<HistoryState> emit) {
    final index = state.captureManager.notifier.value;
    emit(state.copyWith(index: index));
  }

  void _clear(ClearHistoryEvent event, Emitter<HistoryState> emit) {
    emit(HistoryState.initailize());
  }

  void _initialize(InitailizeCapture event, Emitter<HistoryState> emit) {
    state.captureManager.initialize(event.state);
  }

  void _capture(CaptureState event, Emitter<HistoryState> emit) {
    state.captureManager.captureState(event.state);
  }

  void _redo(RedoEvent event, Emitter<HistoryState> emit) {
    state.captureManager.redo()?.run();
  }

  void _undo(UndoEvent event, Emitter<HistoryState> emit) {
    state.captureManager.undo()?.run();
  }
}
