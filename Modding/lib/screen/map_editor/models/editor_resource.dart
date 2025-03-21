import 'package:audioplayers/audioplayers.dart';
import 'package:custom_mouse_cursor/custom_mouse_cursor.dart';
import 'package:equatable/equatable.dart';

class EditorResource extends Equatable {
  const EditorResource({
    this.eraseCursor,
    this.panCursor,
    this.multiSelectCursor,
    required this.pickItemSound,
    required this.removeItemSound,
    required this.setItemSound,
    required this.mapLoadedSound,
    required this.clearMapSound,
    required this.switchResourceSound,
  });

  final CustomMouseCursor? eraseCursor;

  final CustomMouseCursor? panCursor;

  final CustomMouseCursor? multiSelectCursor;

  final AudioPlayer pickItemSound;

  final AudioPlayer removeItemSound;

  final AudioPlayer setItemSound;

  final AudioPlayer mapLoadedSound;

  final AudioPlayer clearMapSound;

  final AudioPlayer switchResourceSound;

  @override
  List<Object?> get props {
    return [
      eraseCursor,
      panCursor,
      multiSelectCursor,
      pickItemSound,
      removeItemSound,
      setItemSound,
      mapLoadedSound,
      clearMapSound,
      switchResourceSound,
    ];
  }
}
