import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

part 'mouse_cursor_event.dart';
part 'mouse_cursor_state.dart';

class MouseCursorBloc extends Bloc<MouseCursorEvent, MouseCursorState> {
  MouseCursorBloc() : super(const MouseCursorState(cursor: MouseCursor.defer)) {
    on<ChangeCursorEvent>(_changeCursor);
  }

  void _changeCursor(ChangeCursorEvent event, Emitter<MouseCursorState> emit) {
    emit(MouseCursorState(cursor: event.cursor));
  }
}
