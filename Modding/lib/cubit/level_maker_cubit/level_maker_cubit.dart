import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:shared_preferences/shared_preferences.dart';

part 'level_maker_state.dart';

class LevelMakerCubit extends Cubit<LevelMakerState> {
  LevelMakerCubit() : super(LevelMakerState.initialize()) {
    _loadPreferences();
  }

  void _loadPreferences() async {
    final prefs = await SharedPreferences.getInstance();
    emit(state.copyWith(
      resourceLocation: prefs.getString('levelMaker.resourceLocation'),
    ));
  }

  Future<void> setResourceLocation(
    String resourceLocation,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('levelMaker.resourceLocation', resourceLocation);
    emit(state.copyWith(resourceLocation: resourceLocation));
  }
}
