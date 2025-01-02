import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:shared_preferences/shared_preferences.dart';

final jsProvider = StateNotifierProvider<JsProviderNotifier, JsProvider>(
  (ref) => JsProviderNotifier(),
);

class JsProvider {
  final bool showConfirmDialog;
  JsProvider({
    this.showConfirmDialog = false,
  });

  JsProvider copyWith({
    bool? showConfirmDialog,
  }) {
    return JsProvider(
      showConfirmDialog: showConfirmDialog ?? this.showConfirmDialog,
    );
  }
}

class JsProviderNotifier extends StateNotifier<JsProvider> {
  JsProviderNotifier() : super(JsProvider()) {
    _loadPreferences();
  }

  void _loadPreferences() async {
    final prefs = await SharedPreferences.getInstance();
    final showConfirmDialog = prefs.getBool('js.showConfirmDialog') ?? false;
    state.copyWith(showConfirmDialog: showConfirmDialog);
  }

  void setshowConfirmDialog(bool showConfirmDialog) {
    state = state.copyWith(showConfirmDialog: showConfirmDialog);
  }
}
