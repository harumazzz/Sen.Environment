part of 'initial_directory_bloc.dart';

@immutable
sealed class InitialDirectoryState extends Equatable {
  const InitialDirectoryState();

  @override
  List<Object?> get props => [];
}

class InitialDirectoryEmpty extends InitialDirectoryState {
  const InitialDirectoryEmpty();
}

class InitialDirectoryLoading extends InitialDirectoryState {
  const InitialDirectoryLoading();
}

class InitialDirectoryLoaded extends InitialDirectoryState {
  const InitialDirectoryLoaded({required this.initialDirectory});
  final String initialDirectory;

  @override
  List<Object?> get props => [initialDirectory];
}

class InitialDirectoryError extends InitialDirectoryState {
  const InitialDirectoryError({required this.message});
  final String message;

  @override
  List<Object?> get props => [message];
}
