part of 'autosave_bloc.dart';

@immutable
class AutosaveState extends Equatable {
  final List<String> files;

  const AutosaveState({
    required this.files,
  });

  @override
  List<Object?> get props => [files];
}
