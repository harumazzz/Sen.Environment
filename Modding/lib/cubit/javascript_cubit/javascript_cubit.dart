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
    state.copyWith(
      showConfirmDialog: prefs.getBool('jsShowConfirmDialog') ?? true,
      runAsLauncher: prefs.getBool('jsRunAsLauncher') ?? false,
    );
  }

  Future<void> setShowConfirmDialog(
    bool showConfirmDialog,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('jsShowConfirmDialog', showConfirmDialog);
    emit(state.copyWith(showConfirmDialog: showConfirmDialog));
  }

  Future<void> setRunAsLauncher(
    bool runAsLauncher,
  ) async {
    final prefs = await SharedPreferences.getInstance();
    await prefs.setBool('jsRunAsLauncher', runAsLauncher);
    emit(state.copyWith(runAsLauncher: runAsLauncher));
  }
}
