import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:shared_preferences/shared_preferences.dart';

//copy form level provider

final mapProvider = StateNotifierProvider<MapProvider, MapState>(
  (ref) => MapProvider(),
);

class MapState {
  final String? resourceLocation;
  final bool isLoading;

  MapState({
    this.resourceLocation,
    this.isLoading = true,
  });

  MapState copyWith({
    String? resourceLocation,
    bool? isLoading,
  }) {
    return MapState(
      resourceLocation: resourceLocation ?? this.resourceLocation,
      isLoading: isLoading ?? this.isLoading,
    );
  }
}

class MapProvider extends StateNotifier<MapState> {
  MapProvider() : super(MapState()) {
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
