part of 'interaction_bloc.dart';

sealed class InteractionEvent extends Equatable {
  const InteractionEvent();

  @override
  List<Object?> get props => [];
}

final class BooleanInputEvent extends InteractionEvent {
  final Completer<String?> completer;

  const BooleanInputEvent({required this.completer});

  @override
  List<Object?> get props => [completer];
}

final class StringInputEvent extends InteractionEvent {
  final Completer<String?> completer;

  const StringInputEvent({required this.completer});

  @override
  List<Object?> get props => [completer];
}

final class PickLocalStorage extends InteractionEvent {
  final Completer<String?> completer;

  const PickLocalStorage({required this.completer});

  @override
  List<Object?> get props => [completer];
}

final class StringInputCompleteEvent extends InteractionEvent {
  final String? value;

  const StringInputCompleteEvent({required this.value});
  @override
  List<Object?> get props => [value];
}

final class BooleanInputCompleteEvent extends InteractionEvent {
  final bool? value;

  const BooleanInputCompleteEvent({required this.value});
  @override
  List<Object?> get props => [value];
}

final class EnumerationSelectEvent extends InteractionEvent {
  final Completer<String?> completer;

  const EnumerationSelectEvent({required this.completer});

  @override
  List<Object?> get props => [completer];
}

final class EnumerationSelectCompleteEvent extends InteractionEvent {
  final String? value;

  const EnumerationSelectCompleteEvent({required this.value});
  @override
  List<Object?> get props => [value];
}
