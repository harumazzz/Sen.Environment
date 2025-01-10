import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/service/file_helper.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

part 'load_script_event.dart';
part 'load_script_state.dart';

class LoadScriptBloc extends Bloc<LoadScriptEvent, LoadScriptState> {
  final SettingsCubit settingsCubit;
  final AppLocalizations localizations;

  LoadScriptBloc({
    required this.settingsCubit,
    required this.localizations,
  }) : super(LoadScriptInitial()) {
    on<LoadScripts>(_loadScript);
    on<ReloadScripts>(_reloadScript);
  }

  void _loadScript(
    LoadScripts event,
    Emitter<LoadScriptState> emit,
  ) async {
    return await _loadScriptFile(emit);
  }

  Future<void> _loadScriptFile(
    Emitter<LoadScriptState> emit,
  ) async {
    emit(LoadScriptLoading());
    try {
      final scriptPath = '${settingsCubit.state.toolChain}/Script/Helper/script.json';
      if (FileHelper.isFile(scriptPath)) {
        final json = await FileHelper.readJsonAsync(source: scriptPath);
        emit(LoadScriptLoaded.fromJson(json));
      } else {
        throw Exception(localizations.script_not_found);
      }
    } catch (e) {
      emit(LoadScriptFailed(message: e.toString()));
    }
  }

  void _reloadScript(
    ReloadScripts event,
    Emitter<LoadScriptState> emit,
  ) async {
    return await _loadScriptFile(emit);
  }
}
