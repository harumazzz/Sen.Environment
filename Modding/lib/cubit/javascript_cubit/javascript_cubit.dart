import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:shared_preferences/shared_preferences.dart';

part 'javascript_state.dart';

class JavascriptCubit extends Cubit<JavascriptState> {
  JavascriptCubit() : super(JavascriptState.initialize()) {
    _loadPreferences();
  }

  void _loadPreferences() async {
    final prefs = await SharedPreferences.getInstance();
    final showConfirmDialog = prefs.getBool('js.showConfirmDialog') ?? false;
    state.copyWith(showConfirmDialog: showConfirmDialog);
  }

  Future<void> setshowConfirmDialog(
    bool showConfirmDialog,
  ) async {
    emit(state.copyWith(showConfirmDialog: showConfirmDialog));
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('js.showConfirmDialog', showConfirmDialog);
  }
}
