import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:shared_preferences/shared_preferences.dart';

final levelProvider = StateNotifierProvider<LevelProvider, LevelState>(
  (ref) => LevelProvider(),
);

class LevelState {
  String? resourceLocation;
  LevelState({
    this.resourceLocation,
  });

  LevelState copyWith({
    String? resourceLocation,
  }) {
    return LevelState(
      resourceLocation: resourceLocation ?? this.resourceLocation,
    );
  }
}

class LevelProvider extends StateNotifier<LevelState> {
  LevelProvider() : super(LevelState()) {
    _loadPreferences();
  }

  Future<void> _loadPreferences() async {
    final prefs = await SharedPreferences.getInstance();
    state = state.copyWith(
      resourceLocation: prefs.getString('resourceLocation'),
    );
  }

  Future<void> setResourceLocation(String resourceLocation) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('resourceLocation', resourceLocation);
    state = state.copyWith(resourceLocation: resourceLocation);
  }
}
