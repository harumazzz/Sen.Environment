part of 'interaction_bloc.dart';

sealed class InteractionEvent extends Equatable {
  const InteractionEvent();

  @override
  List<Object?> get props => <Object>[];
}

final class BooleanInputEvent extends InteractionEvent {
  const BooleanInputEvent({required this.completer});

  final Completer<String?> completer;

  @override
  List<Object?> get props => [completer];
}

final class StringInputEvent extends InteractionEvent {
  const StringInputEvent({required this.completer});
  final Completer<String?> completer;

  @override
  List<Object?> get props => [completer];
}

final class PickLocalStorage extends InteractionEvent {
  const PickLocalStorage({required this.completer});
  final Completer<String?> completer;

  @override
  List<Object?> get props => [completer];
}

final class StringInputCompleteEvent extends InteractionEvent {
  const StringInputCompleteEvent({required this.value});
  final String? value;
  @override
  List<Object?> get props => [value];
}

final class BooleanInputCompleteEvent extends InteractionEvent {
  const BooleanInputCompleteEvent({required this.value});
  final bool? value;
  @override
  List<Object?> get props => [value];
}

final class EnumerationSelectEvent extends InteractionEvent {
  const EnumerationSelectEvent({required this.completer});
  final Completer<String?> completer;

  @override
  List<Object?> get props => [completer];
}

final class EnumerationSelectCompleteEvent extends InteractionEvent {
  const EnumerationSelectCompleteEvent({required this.value});
  final String? value;
  @override
  List<Object?> get props => [value];
}
