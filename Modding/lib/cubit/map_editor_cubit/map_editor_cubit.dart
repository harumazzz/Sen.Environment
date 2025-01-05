import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:shared_preferences/shared_preferences.dart';

part 'map_editor_state.dart';

class MapEditorCubit extends Cubit<MapEditorState> {
  MapEditorCubit() : super(MapEditorState.initialize()) {
    _loadPreferences();
  }

  void _loadPreferences() async {
    final prefs = await SharedPreferences.getInstance();
    emit(state.copyWith(
      resourceLocation: prefs.getString('mapEditor.resourceLocation'),
    ));
  }

  Future<void> setResourceLocation(
    String resourceLocation,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('mapEditor.resourceLocation', resourceLocation);
    emit(state.copyWith(resourceLocation: resourceLocation));
  }
}
