import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

part 'settings_state.dart';

class SettingsCubit extends Cubit<SettingsState> {
  SettingsCubit() : super(SettingsState.initialize()) {
    _loadPreferences();
  }

  ThemeMode get themeData {
    final Map<String, ThemeMode> exchanger = {
      'system': ThemeMode.system,
      'dark': ThemeMode.dark,
      'light': ThemeMode.light,
    };
    return exchanger[state.theme] ?? ThemeMode.system;
  }

  void _loadPreferences() async {
    final prefs = await SharedPreferences.getInstance();
    emit(state.copyWith(
      theme: prefs.getString('theme') ?? 'system',
      locale: prefs.getString('locale') ?? 'en',
      sendNotification: prefs.getBool('sendNotification') ?? false,
      toolChain: prefs.getString('toolchain') ?? '',
      isValid: prefs.getBool('isValid') ?? false,
      requestedPermission: prefs.getBool('requestedPermission') ?? false,
    ));
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

  Future<void> setNotification(bool sendNotification) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('sendNotification', sendNotification);
    emit(state.copyWith(sendNotification: sendNotification));
  }

  Future<void> setToolChain(String toolChain) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('toolchain', toolChain);
    emit(state.copyWith(toolChain: toolChain));
  }

  Future<void> setIsValid(bool isValid) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('isValid', isValid);
    emit(state.copyWith(isValid: isValid));
  }

  Future<void> setRequestedPermission(bool requestedPermission) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('requestedPermission', requestedPermission);
    emit(state.copyWith(requestedPermission: requestedPermission));
  }
}
