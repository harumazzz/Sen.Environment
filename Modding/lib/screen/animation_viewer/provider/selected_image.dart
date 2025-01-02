import 'package:flutter_riverpod/flutter_riverpod.dart';

class SelectImageListNotifier extends StateNotifier<List<bool>> {
  SelectImageListNotifier() : super([]);

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

  void enableAll() {
    state = List.generate(state.length, (index) => true);
  }

  void disableAll() {
    state = List.generate(state.length, (index) => false);
  }
}

final selectedImageListProvider = StateNotifierProvider<SelectImageListNotifier, List<bool>>(
  (ref) => SelectImageListNotifier(),
);
