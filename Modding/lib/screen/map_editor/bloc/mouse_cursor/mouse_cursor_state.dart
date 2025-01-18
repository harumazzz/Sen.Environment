part of 'mouse_cursor_bloc.dart';

final class MouseCursorState extends Equatable {
  const MouseCursorState({required this.cursor});

  final MouseCursor cursor;

  @override
  List<Object?> get props => [cursor];
}
