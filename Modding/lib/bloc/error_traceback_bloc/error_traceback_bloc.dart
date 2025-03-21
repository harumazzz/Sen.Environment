import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';
import '../../model/message.dart';

part 'error_traceback_event.dart';
part 'error_traceback_state.dart';

class ErrorTracebackBloc
    extends Bloc<ErrorTracebackEvent, ErrorTracebackState> {
  ErrorTracebackBloc() : super(const ErrorTracebackInitial(errors: [])) {
    on<ErrorTracebackMessageEvent>(_onError);
    on<ErrorTracebackClearEvent>(_onClear);
  }

  void _onError(
    ErrorTracebackMessageEvent event,
    Emitter<ErrorTracebackState> emit,
  ) {
    emit(ErrorTracebackMessage(errors: [...state.errors, event.message]));
  }

  void _onClear(
    ErrorTracebackClearEvent event,
    Emitter<ErrorTracebackState> emit,
  ) {
    emit(const ErrorTracebackNone(errors: []));
  }
}
