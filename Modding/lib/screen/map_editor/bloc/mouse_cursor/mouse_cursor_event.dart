part of 'mouse_cursor_bloc.dart';

sealed class MouseCursorEvent extends Equatable {
  const MouseCursorEvent();

  @override
  List<Object> get props => [];
}

final class ChangeCursorEvent extends MouseCursorEvent {
  const ChangeCursorEvent({required this.cursor});

  final MouseCursor cursor;

  @override
  List<Object> get props => [cursor];
}
