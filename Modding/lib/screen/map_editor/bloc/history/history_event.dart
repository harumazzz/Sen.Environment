part of 'history_bloc.dart';

sealed class HistoryEvent extends Equatable {
  const HistoryEvent();

  @override
  List<Object> get props => [];
}

final class InitailizeCapture extends HistoryEvent {
  const InitailizeCapture({required this.state});
  final ActionService<ActionModelType> state;

  @override
  List<Object> get props => [state];
}

final class CaptureState extends HistoryEvent {
  const CaptureState({required this.state});

  final ActionService<ActionModelType> state;

  @override
  List<Object> get props => [state];
}

final class RedoEvent extends HistoryEvent {
  const RedoEvent();

  @override
  List<Object> get props => [];
}

final class UndoEvent extends HistoryEvent {
  const UndoEvent();

  @override
  List<Object> get props => [];
}

final class ClearHistoryEvent extends HistoryEvent {
  const ClearHistoryEvent();

  @override
  List<Object> get props => [];
}

final class UpdateIndexEvent extends HistoryEvent {
  const UpdateIndexEvent();

  @override
  List<Object> get props => [];
}
