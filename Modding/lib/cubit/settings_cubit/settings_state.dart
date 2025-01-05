part of 'settings_cubit.dart';

class SettingsState extends Equatable {
  final String theme;
  final String locale;
  final bool sendNotification;
  final String toolChain;
  final bool isValid;
  final bool requestedPermission;

  const SettingsState({
    required this.theme,
    required this.locale,
    required this.sendNotification,
    required this.toolChain,
    required this.isValid,
    required this.requestedPermission,
  });

  factory SettingsState.initialize() {
    return const SettingsState(
      theme: 'system',
      locale: 'en',
      sendNotification: false,
      toolChain: '',
      isValid: false,
      requestedPermission: false,
    );
  }

  SettingsState copyWith({
    String? theme,
    String? locale,
    bool? sendNotification,
    String? toolChain,
    bool? isValid,
    bool? requestedPermission,
  }) {
    return SettingsState(
      theme: theme ?? this.theme,
      locale: locale ?? this.locale,
      sendNotification: sendNotification ?? this.sendNotification,
      toolChain: toolChain ?? this.toolChain,
      isValid: isValid ?? this.isValid,
      requestedPermission: requestedPermission ?? this.requestedPermission,
    );
  }

  @override
  List<Object?> get props => [theme, locale, sendNotification, toolChain, isValid, requestedPermission];

  @override
  String toString() {
    return 'SettingsState(theme: $theme, locale: $locale, sendNotification: $sendNotification, toolChain: $toolChain, isValid: $isValid, requestedPermission: $requestedPermission)';
  }
}
