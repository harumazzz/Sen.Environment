part of 'initial_directory_bloc.dart';

@immutable
sealed class InitialDirectoryEvent extends Equatable {
  const InitialDirectoryEvent();

  @override
  List<Object?> get props => [];
}

class SetDirectoryOfFile extends InitialDirectoryEvent {
  const SetDirectoryOfFile({required this.source});
  final String source;

  @override
  List<Object?> get props => [source];
}

class SetDirectoryOfDirectory extends InitialDirectoryEvent {
  const SetDirectoryOfDirectory({required this.source});
  final String source;

  @override
  List<Object?> get props => [source];
}

class ResetDirectory extends InitialDirectoryEvent {
  const ResetDirectory();
}
