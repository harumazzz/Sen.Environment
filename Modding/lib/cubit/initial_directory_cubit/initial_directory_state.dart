part of 'initial_directory_cubit.dart';

class InitialDirectoryState extends Equatable {
  const InitialDirectoryState({this.initialDirectory});

  factory InitialDirectoryState.initialize() {
    return const InitialDirectoryState();
  }
  final String? initialDirectory;

  InitialDirectoryState copyWith({required String? initialDirectory}) {
    return InitialDirectoryState(initialDirectory: initialDirectory);
  }

  @override
  String toString() =>
      'InitialDirectoryState(initialDirectory: $initialDirectory)';

  @override
  List<Object?> get props => [initialDirectory];
}
