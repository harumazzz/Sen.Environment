import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../models/config.dart';

part 'setting_event.dart';
part 'setting_state.dart';

class SettingBloc extends Bloc<SettingEvent, SettingState> {
  SettingBloc() : super(SettingState.initailize()) {
    on<SetPlaySingleFrame>(_setPlaySingleFrame);
    on<SetMuteAudio>(_setMuteAudio);
    on<SetFilterQuality>(_setFilterQuality);
    on<SetBoundBackground>(_setBoundBackground);
    on<SetIslandImageBorder>(_setIslandImageBorder);
    on<SetIslandAnimationBorder>(_setIslandAnimationBorder);
    on<SetEventBorder>(_setEventBorder);
    on<SetHideMissingArt>(_setHideMissingArt);
    on<SetHideOldEvent>(_setHideOldEvent);
    on<SetPlantCostume>(_setPlantCostume);
    on<SetMapPath>(_setMapPath);
    on<SetMapCompleted>(_setLevelFinished);
    on<SetMapGrid>(_setMapGrid);
    on<SetBoundingColor>(_setBoundingColor);
    on<SetShowScreenShortcut>(_setShowScreenShortcut);
  }

  void _setPlaySingleFrame(
    SetPlaySingleFrame event,
    Emitter<SettingState> emit,
  ) {
    emit(state.copyWith(playSingleFrame: event.playSingleFrame));
  }

  void _setMuteAudio(SetMuteAudio event, Emitter<SettingState> emit) {
    emit(state.copyWith(muteAudio: event.muteAudio));
  }

  void _setFilterQuality(SetFilterQuality event, Emitter<SettingState> emit) {
    emit(state.copyWith(filterQuality: event.filterQuality));
  }

  void _setBoundBackground(
    SetBoundBackground event,
    Emitter<SettingState> emit,
  ) {
    emit(state.copyWith(boundBackground: event.background));
  }

  void _setShowScreenShortcut(
    SetShowScreenShortcut event,
    Emitter<SettingState> emit,
  ) {
    emit(state.copyWith(showTopScreenShortcut: event.enabled));
  }

  void _setIslandImageBorder(
    SetIslandImageBorder event,
    Emitter<SettingState> emit,
  ) {
    emit(state.copyWith(islandImageBorder: event.enabled));
  }

  void _setIslandAnimationBorder(
    SetIslandAnimationBorder event,
    Emitter<SettingState> emit,
  ) {
    emit(state.copyWith(islandAnimationBorder: event.enabled));
  }

  void _setEventBorder(SetEventBorder event, Emitter<SettingState> emit) {
    emit(state.copyWith(eventBorder: event.enabled));
  }

  void _setHideMissingArt(SetHideMissingArt event, Emitter<SettingState> emit) {
    emit(state.copyWith(hideMissingArt: event.enabled));
  }

  void _setHideOldEvent(SetHideOldEvent event, Emitter<SettingState> emit) {
    emit(state.copyWith(hideOldEvent: event.enabled));
  }

  void _setPlantCostume(SetPlantCostume event, Emitter<SettingState> emit) {
    emit(state.copyWith(plantCostume: event.enabled));
  }

  void _setMapPath(SetMapPath event, Emitter<SettingState> emit) {
    emit(state.copyWith(mapPath: event.enabled));
  }

  void _setLevelFinished(SetMapCompleted event, Emitter<SettingState> emit) {
    emit(state.copyWith(mapCompleted: event.enabled));
  }

  void _setMapGrid(SetMapGrid event, Emitter<SettingState> emit) {
    emit(state.copyWith(mapGrid: event.enabled));
  }

  void _setBoundingColor(SetBoundingColor event, Emitter<SettingState> emit) {
    emit(state.copyWith(boundingColor: event.color));
  }
}
