import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';
import '../../bloc/settings_bloc/settings_bloc.dart';
import '../../service/file_helper.dart';
import '../../i18n/app_localizations.dart';

part 'load_script_event.dart';
part 'load_script_state.dart';

class LoadScriptBloc extends Bloc<LoadScriptEvent, LoadScriptState> {
  LoadScriptBloc({required this.settingsBloc}) : super(LoadScriptInitial()) {
    on<LoadScripts>(_loadScript);
    on<ReloadScripts>(_reloadScript);
    on<SearchScripts>(_searchScript);
  }
  final SettingsBloc settingsBloc;

  void _loadScript(LoadScripts event, Emitter<LoadScriptState> emit) async {
    return await _loadScriptFile(event, emit);
  }

  Future<void> _loadScriptFile(
    LoadScriptEvent event,
    Emitter<LoadScriptState> emit,
  ) async {
    emit(LoadScriptLoading());
    try {
      final scriptPath =
          '${settingsBloc.state.toolChain}/Script/Helper/script.json';
      if (FileHelper.isFile(scriptPath)) {
        final json = await FileHelper.readJsonAsync(source: scriptPath);
        emit(LoadScriptLoaded.fromJson(json));
      } else {
        throw Exception(event.localizations.script_not_found);
      }
    } catch (e) {
      emit(LoadScriptFailed(message: e.toString()));
    }
  }

  void _reloadScript(ReloadScripts event, Emitter<LoadScriptState> emit) async {
    return await _loadScriptFile(event, emit);
  }

  void _searchScript(SearchScripts event, Emitter<LoadScriptState> emit) {
    final currentState = state;
    if (currentState is LoadScriptLoaded) {
      final filtered = [
        ...currentState.allData.where(
          (script) =>
              script.name.toLowerCase().contains(event.query.toLowerCase()),
        ),
      ];
      emit(currentState.copyWith(filteredData: filtered));
    }
  }
}
