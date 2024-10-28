import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:shared_preferences/shared_preferences.dart';

final settingProvider = StateNotifierProvider<SettingProvider, SettingState>(
  (ref) => SettingProvider(),
);

class SettingState {
  final String theme;
  final String locale;
  final bool sendNotification;
  final String toolChain;
  final bool isValid;
  final bool requestedPermission;

  SettingState({
    this.theme = 'system',
    this.locale = 'en',
    this.sendNotification = false,
    this.toolChain = '',
    this.isValid = false,
    this.requestedPermission = false,
  });

  ThemeMode get themeData {
    final Map<String, ThemeMode> exchanger = {
      'system': ThemeMode.system,
      'dark': ThemeMode.dark,
      'light': ThemeMode.light,
    };
    return exchanger[theme] ?? ThemeMode.system;
  }

  SettingState copyWith({
    String? theme,
    String? locale,
    bool? sendNotification,
    String? toolChain,
    bool? isValid,
    bool? requestedPermission,
  }) {
    return SettingState(
      theme: theme ?? this.theme,
      locale: locale ?? this.locale,
      sendNotification: sendNotification ?? this.sendNotification,
      toolChain: toolChain ?? this.toolChain,
      isValid: isValid ?? this.isValid,
      requestedPermission: requestedPermission ?? this.requestedPermission,
    );
  }
}

class SettingProvider extends StateNotifier<SettingState> {
  SettingProvider() : super(SettingState()) {
    _loadPreferences();
  }

  Future<void> _loadPreferences() async {
    final prefs = await SharedPreferences.getInstance();

    state = state.copyWith(
      theme: prefs.getString('theme') ?? 'system',
      locale: prefs.getString('locale') ?? 'en',
      sendNotification: prefs.getBool('sendNotification') ?? false,
      toolChain: prefs.getString('toolchain') ?? '',
      isValid: prefs.getBool('isValid') ?? false,
      requestedPermission: prefs.getBool('requestedPermission') ?? false,
    );
  }

  Future<void> setTheme(String theme) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('theme', theme);
    state = state.copyWith(theme: theme);
  }

  Future<void> setLocale(String locale) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('locale', locale);
    state = state.copyWith(locale: locale);
  }

  Future<void> setNotification(bool sendNotification) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('sendNotification', sendNotification);
    state = state.copyWith(sendNotification: sendNotification);
  }

  Future<void> setToolChain(String toolChain) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('toolchain', toolChain);
    state = state.copyWith(toolChain: toolChain);
  }

  Future<void> setIsValid(bool isValid) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('isValid', isValid);
    state = state.copyWith(isValid: isValid);
  }

  Future<void> setRequestedPermission(bool requestedPermission) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('requestedPermission', requestedPermission);
    state = state.copyWith(requestedPermission: requestedPermission);
  }
}
