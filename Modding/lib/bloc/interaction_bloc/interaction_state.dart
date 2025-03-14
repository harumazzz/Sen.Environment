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
  const StringInputState({required this.completer});
  final Completer<String?> completer;

  @override
  List<Object?> get props => [completer];
}

final class BooleanInputState extends InteractionState {
  const BooleanInputState({required this.completer});
  final Completer<String?> completer;

  @override
  List<Object?> get props => [completer];
}

final class EnumerationSelectState extends InteractionState {
  const EnumerationSelectState({required this.completer});
  final Completer<String?> completer;

  @override
  List<Object?> get props => [completer];
}

final class BlockInteractionState extends InteractionState {
  const BlockInteractionState();
}
