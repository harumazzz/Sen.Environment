part of 'setting_bloc.dart';

final class SettingState extends Equatable {
  const SettingState({
    required this.boundBackground,
    required this.showTopScreenShortcut,
    required this.islandImageBorder,
    required this.islandAnimationBorder,
    required this.eventBorder,
    required this.hideMissingArt,
    required this.hideOldEvent,
    required this.mapPath,
    required this.plantCostume,
    required this.mapCompleted,
    required this.mapGrid,
    required this.boundingColor,
    required this.playSingleFrame,
    required this.muteAudio,
    required this.filterQuality,
  });

  factory SettingState.initailize() {
    return const SettingState(
      boundBackground: BorderBackground.color,
      showTopScreenShortcut: true,
      islandImageBorder: true,
      islandAnimationBorder: true,
      eventBorder: true,
      hideMissingArt: true,
      hideOldEvent: true,
      mapPath: true,
      plantCostume: true,
      mapCompleted: true,
      mapGrid: false,
      boundingColor: Colors.black,
      playSingleFrame: false,
      muteAudio: false,
      filterQuality: FilterQuality.high,
    );
  }

  final BorderBackground boundBackground;

  final bool showTopScreenShortcut;

  final bool islandImageBorder;

  final bool islandAnimationBorder;

  final bool eventBorder;

  final bool hideMissingArt;

  final bool hideOldEvent;

  final bool mapPath;

  final bool plantCostume;

  final bool mapCompleted;

  final bool mapGrid;

  final Color boundingColor;

  final bool playSingleFrame;

  final bool muteAudio;

  final FilterQuality filterQuality;

  SettingState copyWith({
    BorderBackground? boundBackground,
    bool? showTopScreenShortcut,
    bool? islandImageBorder,
    bool? islandAnimationBorder,
    bool? eventBorder,
    bool? hideMissingArt,
    bool? hideOldEvent,
    bool? mapPath,
    bool? plantCostume,
    bool? mapCompleted,
    bool? mapGrid,
    Color? boundingColor,
    bool? playSingleFrame,
    bool? muteAudio,
    FilterQuality? filterQuality,
  }) {
    return SettingState(
      showTopScreenShortcut:
          showTopScreenShortcut ?? this.showTopScreenShortcut,
      boundBackground: boundBackground ?? this.boundBackground,
      islandImageBorder: islandImageBorder ?? this.islandImageBorder,
      islandAnimationBorder:
          islandAnimationBorder ?? this.islandAnimationBorder,
      eventBorder: eventBorder ?? this.eventBorder,
      hideMissingArt: hideMissingArt ?? this.hideMissingArt,
      hideOldEvent: hideOldEvent ?? this.hideOldEvent,
      mapPath: mapPath ?? this.mapPath,
      plantCostume: plantCostume ?? this.plantCostume,
      mapCompleted: mapCompleted ?? this.mapCompleted,
      mapGrid: mapGrid ?? this.mapGrid,
      boundingColor: boundingColor ?? this.boundingColor,
      playSingleFrame: playSingleFrame ?? this.playSingleFrame,
      muteAudio: muteAudio ?? this.muteAudio,
      filterQuality: filterQuality ?? this.filterQuality,
    );
  }

  @override
  List<Object?> get props => [
    boundBackground,
    showTopScreenShortcut,
    islandImageBorder,
    islandAnimationBorder,
    muteAudio,
    hideMissingArt,
    hideOldEvent,
    plantCostume,
    mapPath,
    mapCompleted,
    eventBorder,
    mapGrid,
    boundingColor,
    playSingleFrame,
    filterQuality,
  ];
}
