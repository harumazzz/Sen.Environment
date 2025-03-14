part of 'autosave_bloc.dart';

@immutable
class AutosaveState extends Equatable {
  const AutosaveState({required this.files});
  final List<String> files;

  @override
  List<Object?> get props => [files];
}
