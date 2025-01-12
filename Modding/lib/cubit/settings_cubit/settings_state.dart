part of 'settings_cubit.dart';

class SettingsState extends Equatable {
  final String theme;
  final String locale;
  final bool sendNotification;
  final String toolChain;
  final bool isValid;
  final bool requestedPermission;
  final String mapEditorResource;
  final String levelMakerResource;
  final bool shellLaunchImmediately;
  final bool jsShowConfirmDialog;
  final bool jsRunAsLauncher;

  const SettingsState({
    required this.theme,
    required this.locale,
    required this.sendNotification,
    required this.toolChain,
    required this.isValid,
    required this.requestedPermission,
    required this.mapEditorResource,
    required this.levelMakerResource,
    required this.shellLaunchImmediately,
    required this.jsShowConfirmDialog,
    required this.jsRunAsLauncher,
  });

  factory SettingsState.initialize() {
    return const SettingsState(
      theme: 'system',
      locale: 'en',
      sendNotification: false,
      toolChain: '',
      isValid: false,
      requestedPermission: false,
      mapEditorResource: '',
      levelMakerResource: '',
      shellLaunchImmediately: false,
      jsRunAsLauncher: false,
      jsShowConfirmDialog: true,
    );
  }

  SettingsState copyWith({
    String? theme,
    String? locale,
    bool? sendNotification,
    String? toolChain,
    bool? isValid,
    bool? requestedPermission,
    String? mapEditorResource,
    String? levelMakerResource,
    bool? shellLaunchImmediately,
    bool? jsRunAsLauncher,
    bool? jsShowConfirmDialog,
  }) {
    return SettingsState(
      theme: theme ?? this.theme,
      locale: locale ?? this.locale,
      sendNotification: sendNotification ?? this.sendNotification,
      toolChain: toolChain ?? this.toolChain,
      isValid: isValid ?? this.isValid,
      requestedPermission: requestedPermission ?? this.requestedPermission,
      mapEditorResource: mapEditorResource ?? this.mapEditorResource,
      levelMakerResource: levelMakerResource ?? this.levelMakerResource,
      shellLaunchImmediately: shellLaunchImmediately ?? this.shellLaunchImmediately,
      jsRunAsLauncher: jsRunAsLauncher ?? this.jsRunAsLauncher,
      jsShowConfirmDialog: jsShowConfirmDialog ?? this.jsShowConfirmDialog,
    );
  }

  @override
  List<Object?> get props => [
        theme,
        locale,
        sendNotification,
        toolChain,
        isValid,
        requestedPermission,
        mapEditorResource,
        levelMakerResource,
        shellLaunchImmediately,
        jsRunAsLauncher,
        jsShowConfirmDialog,
      ];
}
