import 'package:flutter_riverpod/flutter_riverpod.dart';

class SelectedLabelNotifier extends StateNotifier<String> {
  SelectedLabelNotifier() : super('main');

  void setLabel(String newLabel) {
    state = newLabel;
  }

  void resetLabel() {
    state = 'main';
  }
}

final selectedLabel = StateNotifierProvider<SelectedLabelNotifier, String>(
  (ref) => SelectedLabelNotifier(),
);
