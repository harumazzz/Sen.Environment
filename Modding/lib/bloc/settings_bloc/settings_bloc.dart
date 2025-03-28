import 'dart:async';

import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

import '../../constant/localization.dart' as k_locale;
import '../../extension/platform.dart';
import '../../service/android_helper.dart';

part 'settings_event.dart';
part 'settings_state.dart';

class SettingsBloc extends Bloc<SettingsEvent, SettingsState> {
  SettingsBloc() : super(SettingsState.initialize()) {
    on<LoadSettings>(_loadPreferences);
    on<SetTheme>(_setTheme);
    on<SetLocale>(_setLocale);
    on<SetNotification>(_setNotification);
    on<SetToolChain>(_setToolChain);
    on<SetIsValid>(_setIsValid);
    on<SetRequestedPermission>(_setRequestedPermission);
    on<CheckAndroidPermission>(_checkAndroidPermission);
    on<SetRunAsLauncher>(_setRunAsLauncher);
    on<SetLevelMakerResource>(_setLevelMakerResource);
    on<SetShowConfirmDialog>(_setShowConfirmDialog);
    on<SetShellLaunchImmediately>(_setShellLaunchImmediately);
    on<SetMapEditorResource>(_setMapEditorResource);
    on<SetShowAnimationViewerLabel>(_setShowAnimationViewerLabel);
  }

  Future<void> _loadPreferences(
    LoadSettings event,
    Emitter<SettingsState> emit,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    emit(
      state.copyWith(
        theme: prefs.getString('theme') ?? 'system',
        locale: prefs.getString('locale') ?? 'en',
        sendNotification: prefs.getBool('sendNotification') ?? true,
        toolChain: prefs.getString('toolchain') ?? '',
        isValid: prefs.getBool('isValid') ?? false,
        requestedPermission: prefs.getBool('requestedPermission') ?? false,
        jsRunAsLauncher: prefs.getBool('jsRunAsLauncher') ?? false,
        jsShowConfirmDialog: prefs.getBool('jsShowConfirmDialog') ?? true,
        levelMakerResource: prefs.getString('levelMakerResource') ?? '',
        mapEditorResource: prefs.getString('mapEditorResource') ?? '',
        shellLaunchImmediately: prefs.getBool('shellLaunchImmediately') ?? true,
        showAnimationViewerLabel:
            prefs.getBool('showAnimationViewerLabel') ?? true,
      ),
    );
  }

  Future<void> _checkAndroidPermission(
    CheckAndroidPermission event,
    Emitter<SettingsState> emit,
  ) async {
    final permission = await AndroidHelper.checkStoragePermission();
    emit(state.copyWith(requestedPermission: permission));
  }

  Future<void> _setTheme(SetTheme event, Emitter<SettingsState> emit) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('theme', event.theme);
    emit(state.copyWith(theme: event.theme));
  }

  Future<void> _setLocale(SetLocale event, Emitter<SettingsState> emit) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('locale', event.locale);
    emit(state.copyWith(locale: event.locale));
  }

  Future<void> _setNotification(
    SetNotification event,
    Emitter<SettingsState> emit,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('sendNotification', event.sendNotification);
    emit(state.copyWith(sendNotification: event.sendNotification));
  }

  Future<void> _setToolChain(
    SetToolChain event,
    Emitter<SettingsState> emit,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('toolchain', event.toolChain);
    emit(state.copyWith(toolChain: event.toolChain));
  }

  Future<void> _setIsValid(
    SetIsValid event,
    Emitter<SettingsState> emit,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('isValid', event.isValid);
    emit(state.copyWith(isValid: event.isValid));
  }

  Future<void> _setRequestedPermission(
    SetRequestedPermission event,
    Emitter<SettingsState> emit,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('requestedPermission', event.requestedPermission);
    emit(state.copyWith(requestedPermission: event.requestedPermission));
  }

  Future<void> _setRunAsLauncher(
    SetRunAsLauncher event,
    Emitter<SettingsState> emit,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('jsRunAsLauncher', event.value);
    emit(state.copyWith(jsRunAsLauncher: event.value));
  }

  Future<void> _setLevelMakerResource(
    SetLevelMakerResource event,
    Emitter<SettingsState> emit,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('levelMakerResource', event.value);
    emit(state.copyWith(levelMakerResource: event.value));
  }

  Future<void> _setShowConfirmDialog(
    SetShowConfirmDialog event,
    Emitter<SettingsState> emit,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('jsShowConfirmDialog', event.value);
    emit(state.copyWith(jsShowConfirmDialog: event.value));
  }

  Future<void> _setShellLaunchImmediately(
    SetShellLaunchImmediately event,
    Emitter<SettingsState> emit,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('shellLaunchImmediately', event.value);
    emit(state.copyWith(shellLaunchImmediately: event.value));
  }

  Future<void> _setMapEditorResource(
    SetMapEditorResource event,
    Emitter<SettingsState> emit,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('mapEditorResource', event.value);
    emit(state.copyWith(mapEditorResource: event.value));
  }

  Future<void> _setShowAnimationViewerLabel(
    SetShowAnimationViewerLabel event,
    Emitter<SettingsState> emit,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('showAnimationViewerLabel', event.value);
    emit(state.copyWith(showAnimationViewerLabel: event.value));
  }
}
