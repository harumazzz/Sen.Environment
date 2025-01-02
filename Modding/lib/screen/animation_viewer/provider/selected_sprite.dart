import 'package:flutter_riverpod/flutter_riverpod.dart';

class SelectedSpriteListNotifier extends StateNotifier<List<bool>> {
  SelectedSpriteListNotifier() : super([]);

  void reset() {
    state = [];
  }

  void toggle(int index) {
    state = [
      for (var i = 0; i < state.length; i++)
        if (i == index) !state[i] else state[i],
    ];
  }

  void allocate(int size) {
    state = List.generate(size, (index) => true);
  }

  void disableAll() {
    state = List.generate(state.length, (index) => false);
  }

  void enableAll() {
    state = List.generate(state.length, (index) => true);
  }
}

final selectedSpriteListNotifier = StateNotifierProvider<SelectedSpriteListNotifier, List<bool>>(
  (ref) => SelectedSpriteListNotifier(),
);
