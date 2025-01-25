part of 'autosave_bloc.dart';

@immutable
class AutosaveEvent extends Equatable {
  const AutosaveEvent();

  @override
  List<Object?> get props => [];
}

class SaveEvent extends AutosaveEvent {
  const SaveEvent();

  @override
  List<Object?> get props => [];
}

class CleanAutosaveEvent extends AutosaveEvent {
  const CleanAutosaveEvent();

  @override
  List<Object?> get props => [];
}
