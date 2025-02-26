part of 'error_traceback_bloc.dart';

@immutable
sealed class ErrorTracebackState extends Equatable {
  const ErrorTracebackState();

  @override
  List<Object?> get props => [];
}

final class ErrorTracebackInitial extends ErrorTracebackState {
  const ErrorTracebackInitial();
}

final class ErrorTracebackMessage extends ErrorTracebackState {
  final Message message;

  const ErrorTracebackMessage({
    required this.message,
  });

  @override
  List<Object?> get props => [message];
}

final class ErrorTracebackNone extends ErrorTracebackState {
  const ErrorTracebackNone();
}
