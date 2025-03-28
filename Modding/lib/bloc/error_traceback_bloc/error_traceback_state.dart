part of 'error_traceback_bloc.dart';

@immutable
sealed class ErrorTracebackState extends Equatable {
  const ErrorTracebackState({required this.errors});

  final List<Message> errors;

  @override
  List<Object?> get props => [errors];

  bool get hasError => errors.isNotEmpty;

  int get size => errors.length;
}

final class ErrorTracebackInitial extends ErrorTracebackState {
  const ErrorTracebackInitial({required super.errors});
}

final class ErrorTracebackMessage extends ErrorTracebackState {
  const ErrorTracebackMessage({required super.errors});
}

final class ErrorTracebackNone extends ErrorTracebackState {
  const ErrorTracebackNone({required super.errors});
}
