import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';
import '../../extension/platform.dart';
import 'package:shared_preferences/shared_preferences.dart';
import '../../constant/localization.dart' as k_locale;
import '../../service/android_helper.dart';

part 'settings_state.dart';

class SettingsCubit extends Cubit<SettingsState> {
  SettingsCubit() : super(SettingsState.initialize()) {
    _loadPreferences();
  }

  void _loadPreferences() async {
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

  Future<void> setTheme(String theme) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('theme', theme);
    emit(state.copyWith(theme: theme));
  }

  Future<void> setLocale(String locale) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('locale', locale);
    emit(state.copyWith(locale: locale));
  }

  Future<void> setNotification({required bool sendNotification}) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('sendNotification', sendNotification);
    emit(state.copyWith(sendNotification: sendNotification));
  }

  Future<void> setToolChain(String toolChain) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('toolchain', toolChain);
    emit(state.copyWith(toolChain: toolChain));
  }

  Future<void> setIsValid({required bool isValid}) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('isValid', isValid);
    emit(state.copyWith(isValid: isValid));
  }

  Future<void> setRequestedPermission({
    required bool requestedPermission,
  }) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('requestedPermission', requestedPermission);
    emit(state.copyWith(requestedPermission: requestedPermission));
  }

  Future<void> checkAndroidPermission() async {
    return await setRequestedPermission(
      requestedPermission: await AndroidHelper.checkStoragePermission(),
    );
  }

  Future<void> setRunAsLauncher({required bool value}) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('jsRunAsLauncher', value);
    emit(state.copyWith(jsRunAsLauncher: value));
  }

  Future<void> setLevelMakerResource(String value) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('levelMakerResource', value);
    emit(state.copyWith(levelMakerResource: value));
  }

  Future<void> setShowConfirmDialog({required bool value}) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('jsShowConfirmDialog', value);
    emit(state.copyWith(jsShowConfirmDialog: value));
  }

  Future<void> setShellLaunchImmediately({required bool value}) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('shellLaunchImmediately', value);
    emit(state.copyWith(shellLaunchImmediately: value));
  }

  Future<void> setMapEditorResource(String value) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('mapEditorResource', value);
    emit(state.copyWith(mapEditorResource: value));
  }

  Future<void> setShowAnimationViewerLabel({required bool value}) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('showAnimationViewerLabel', value);
    emit(state.copyWith(showAnimationViewerLabel: value));
  }
}
