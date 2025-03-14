part of 'toolbar_bloc.dart';

sealed class ToolBarEvent extends Equatable {
  const ToolBarEvent();

  @override
  List<Object> get props => [];
}

final class ToolEnabled extends ToolBarEvent {
  const ToolEnabled({required this.type});

  final ToolType type;

  @override
  List<Object> get props => [type];
}

final class ToolToogled extends ToolBarEvent {
  const ToolToogled({required this.type, this.enabled});

  final ToolType type;
  final bool? enabled;

  @override
  List<Object> get props => [type];
}

final class ToolOpenEvent extends ToolBarEvent {
  const ToolOpenEvent({
    required this.stageBloc,
    required this.itemBloc,
    required this.layerBloc,
    required this.initialDirectoryCubit,
  });

  final StageBloc stageBloc;

  final ItemBloc itemBloc;

  final LayerBloc layerBloc;

  final InitialDirectoryCubit initialDirectoryCubit;

  @override
  List<Object> get props => [
    stageBloc,
    itemBloc,
    layerBloc,
    initialDirectoryCubit,
  ];
}

final class ToolSaveEvent extends ToolBarEvent {
  const ToolSaveEvent({
    required this.stageBloc,
    required this.initialDirectoryCubit,
    //required this.autosaveBloc,
  });

  final StageBloc stageBloc;

  final InitialDirectoryCubit initialDirectoryCubit;

  //final AutosaveBloc autosaveBloc;

  @override
  List<Object> get props => [stageBloc, initialDirectoryCubit];
}

final class ToolClearEvent extends ToolBarEvent {
  const ToolClearEvent();

  @override
  List<Object> get props => [];
}

final class ToolClearSubmitted extends ToolBarEvent {
  const ToolClearSubmitted({
    required this.stageBloc,
    required this.itemUpdate,
    required this.layerBloc,
  });

  final StageBloc stageBloc;

  final void Function() itemUpdate;

  final LayerBloc layerBloc;

  @override
  List<Object> get props => [stageBloc, itemUpdate, layerBloc];
}

final class ShortcutMenuEvent extends ToolBarEvent {
  const ShortcutMenuEvent();

  @override
  List<Object> get props => [];
}

final class ToolConfigEvent extends ToolBarEvent {
  const ToolConfigEvent();

  @override
  List<Object> get props => [];
}

final class ToolConfigSubmitted extends ToolBarEvent {
  const ToolConfigSubmitted();

  @override
  List<Object> get props => [];
}
