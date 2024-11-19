import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:shared_preferences/shared_preferences.dart';

final levelProvider = StateNotifierProvider<LevelProvider, LevelState>(
  (ref) => LevelProvider(),
);

class LevelState {
  final String? resourceLocation;
  final bool isLoading;

  LevelState({
    this.resourceLocation,
    this.isLoading = true,
  });

  LevelState copyWith({
    String? resourceLocation,
    bool? isLoading,
  }) {
    return LevelState(
      resourceLocation: resourceLocation ?? this.resourceLocation,
      isLoading: isLoading ?? this.isLoading,
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
      isLoading: false,
    );
  }

  Future<void> setResourceLocation(String resourceLocation) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('resourceLocation', resourceLocation);
    state = state.copyWith(resourceLocation: resourceLocation);
  }
}
