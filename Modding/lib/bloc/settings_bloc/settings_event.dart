part of 'settings_bloc.dart';

@immutable
sealed class SettingsEvent extends Equatable {
  const SettingsEvent();

  @override
  List<Object?> get props => [];
}

class LoadSettings extends SettingsEvent {
  const LoadSettings();
}

class SetTheme extends SettingsEvent {
  const SetTheme(this.theme);
  final String theme;

  @override
  List<Object?> get props => [theme];
}

class SetLocale extends SettingsEvent {
  const SetLocale(this.locale);
  final String locale;

  @override
  List<Object?> get props => [locale];
}

class SetNotification extends SettingsEvent {
  const SetNotification({required this.sendNotification});
  final bool sendNotification;

  @override
  List<Object?> get props => [sendNotification];
}

class SetToolChain extends SettingsEvent {
  const SetToolChain(this.toolChain);
  final String toolChain;

  @override
  List<Object?> get props => [toolChain];
}

class SetIsValid extends SettingsEvent {
  const SetIsValid({required this.isValid});
  final bool isValid;

  @override
  List<Object?> get props => [isValid];
}

class SetRequestedPermission extends SettingsEvent {
  const SetRequestedPermission({required this.requestedPermission});
  final bool requestedPermission;

  @override
  List<Object?> get props => [requestedPermission];
}

class CheckAndroidPermission extends SettingsEvent {
  const CheckAndroidPermission();
}

class SetRunAsLauncher extends SettingsEvent {
  const SetRunAsLauncher({required this.value});
  final bool value;

  @override
  List<Object?> get props => [value];
}

class SetLevelMakerResource extends SettingsEvent {
  const SetLevelMakerResource(this.value);
  final String value;

  @override
  List<Object?> get props => [value];
}

class SetShowConfirmDialog extends SettingsEvent {
  const SetShowConfirmDialog({required this.value});
  final bool value;

  @override
  List<Object?> get props => [value];
}

class SetShellLaunchImmediately extends SettingsEvent {
  const SetShellLaunchImmediately({required this.value});
  final bool value;

  @override
  List<Object?> get props => [value];
}

class SetMapEditorResource extends SettingsEvent {
  const SetMapEditorResource(this.value);
  final String value;

  @override
  List<Object?> get props => [value];
}

class SetShowAnimationViewerLabel extends SettingsEvent {
  const SetShowAnimationViewerLabel({required this.value});
  final bool value;

  @override
  List<Object?> get props => [value];
}
