part of 'error_traceback_bloc.dart';

@immutable
sealed class ErrorTracebackEvent extends Equatable {
  const ErrorTracebackEvent();

  @override
  List<Object?> get props => [];
}

final class ErrorTracebackMessageEvent extends ErrorTracebackEvent {
  const ErrorTracebackMessageEvent({required this.message});
  final Message message;

  @override
  List<Object?> get props => [message];
}

final class ErrorTracebackClearEvent extends ErrorTracebackEvent {
  const ErrorTracebackClearEvent();
}
