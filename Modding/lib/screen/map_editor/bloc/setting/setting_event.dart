part of 'setting_bloc.dart';

sealed class SettingEvent extends Equatable {
  const SettingEvent();

  @override
  List<Object> get props => [];
}

final class SetPlaySingleFrame extends SettingEvent {
  const SetPlaySingleFrame({required this.playSingleFrame});

  final bool playSingleFrame;

  @override
  List<Object> get props => [playSingleFrame];
}

final class SetMuteAudio extends SettingEvent {
  const SetMuteAudio({required this.muteAudio});

  final bool muteAudio;

  @override
  List<Object> get props => [muteAudio];
}

final class SetFilterQuality extends SettingEvent {
  const SetFilterQuality({required this.filterQuality});

  final FilterQuality filterQuality;

  @override
  List<Object> get props => [filterQuality];
}

final class SetBoundBackground extends SettingEvent {
  const SetBoundBackground({required this.background});

  final BorderBackground background;

  @override
  List<Object> get props => [background];
}

final class SetShowScreenShortcut extends SettingEvent {
  const SetShowScreenShortcut({required this.enabled});

  final bool enabled;

  @override
  List<Object> get props => [enabled];
}

final class SetIslandImageBorder extends SettingEvent {
  const SetIslandImageBorder({required this.enabled});

  final bool enabled;

  @override
  List<Object> get props => [enabled];
}

final class SetIslandAnimationBorder extends SettingEvent {
  const SetIslandAnimationBorder({required this.enabled});

  final bool enabled;

  @override
  List<Object> get props => [enabled];
}

final class SetEventBorder extends SettingEvent {
  const SetEventBorder({required this.enabled});

  final bool enabled;

  @override
  List<Object> get props => [enabled];
}

final class SetHideMissingArt extends SettingEvent {
  const SetHideMissingArt({required this.enabled});

  final bool enabled;

  @override
  List<Object> get props => [enabled];
}

final class SetHideOldEvent extends SettingEvent {
  const SetHideOldEvent({required this.enabled});

  final bool enabled;

  @override
  List<Object> get props => [enabled];
}

final class SetPlantCostume extends SettingEvent {
  const SetPlantCostume({required this.enabled});

  final bool enabled;

  @override
  List<Object> get props => [enabled];
}

final class SetMapPath extends SettingEvent {
  const SetMapPath({required this.enabled});

  final bool enabled;

  @override
  List<Object> get props => [enabled];
}

final class SetMapCompleted extends SettingEvent {
  const SetMapCompleted({required this.enabled});

  final bool enabled;

  @override
  List<Object> get props => [enabled];
}

final class SetMapGrid extends SettingEvent {
  const SetMapGrid({required this.enabled});

  final bool enabled;

  @override
  List<Object> get props => [enabled];
}

final class SetBoundingColor extends SettingEvent {
  const SetBoundingColor({required this.color});

  final Color color;

  @override
  List<Object> get props => [color];
}
