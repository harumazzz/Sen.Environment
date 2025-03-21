part of 'argument_bloc.dart';

@immutable
sealed class ArgumentEvent extends Equatable {
  const ArgumentEvent();

  @override
  List<Object?> get props => [];
}

final class AddArgument extends ArgumentEvent {
  const AddArgument({required this.value});
  final List<String> value;

  @override
  List<Object?> get props => [value];
}

final class ClearArgument extends ArgumentEvent {
  const ClearArgument();
}

final class RemoveArgument extends ArgumentEvent {
  const RemoveArgument({required this.index});

  final int index;

  @override
  List<Object?> get props => [index];
}
