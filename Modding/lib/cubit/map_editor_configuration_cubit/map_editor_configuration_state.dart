part of 'map_editor_configuration_cubit.dart';

enum AppConfigurationStatus { initial, success, failed }

final class MapEditorConfigurationState extends Equatable {
  // ignore: prefer_const_constructors_in_immutables
  MapEditorConfigurationState({
    this.status = AppConfigurationStatus.initial,
    this.errorSnapShot = '',
  });

  final AppConfigurationStatus status;
  final String errorSnapShot;
  late final String settingPath;
  late final GameResource gameResource;
  late final EditorResource editorResource;
  late final ConfigModel configModel;
  late final HashMap<ToolType, Item> toolItem;
  late final HashMap<SectionType, Item> sectionItem;
  late final HashMap<NavigationType, Item> navigationItem;
  late final HashMap<ExtensionType, Item> extensionItem;
  late final HashMap<ActionType, String> actionTypeLocalization;
  //late final HashMap<RootLayerType, Item> rootLayerItem;

  @override
  List<Object> get props => [status];
}
