part of 'interaction_bloc.dart';

sealed class InteractionState extends Equatable {
  const InteractionState();

  @override
  List<Object?> get props => [];
}

final class InteractionInitial extends InteractionState {
  const InteractionInitial();
}

final class StringInputState extends InteractionState {
  final Completer<String?> completer;

  const StringInputState({
    required this.completer,
  });

  @override
  List<Object?> get props => [completer];
}

final class BooleanInputState extends InteractionState {
  final Completer<String?> completer;

  const BooleanInputState({
    required this.completer,
  });

  @override
  List<Object?> get props => [completer];
}

final class EnumerationSelectState extends InteractionState {
  final Completer<String?> completer;

  const EnumerationSelectState({
    required this.completer,
  });

  @override
  List<Object?> get props => [completer];
}

final class BlockInteractionState extends InteractionState {
  const BlockInteractionState();
}
