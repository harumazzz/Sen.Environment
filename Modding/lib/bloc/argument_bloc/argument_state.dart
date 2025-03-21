part of 'argument_bloc.dart';

@immutable
sealed class ArgumentState extends Equatable {
  const ArgumentState({required this.value});

  final List<String> value;

  @override
  List<Object?> get props => [value];
}

final class ArgumentInitial extends ArgumentState {
  const ArgumentInitial({required super.value});
}

final class ArgumentSubmittedState extends ArgumentState {
  const ArgumentSubmittedState({required super.value});
}

final class ArgumentClearState extends ArgumentState {
  const ArgumentClearState({required super.value});
}

final class ArgumentRemoveState extends ArgumentState {
  const ArgumentRemoveState({required super.value});
}
