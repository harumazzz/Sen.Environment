import 'dart:convert' as converter;
import 'package:path/path.dart' as p;
import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';
import '../../cubit/initial_directory_cubit/initial_directory_cubit.dart';
import '../../service/file_helper.dart';

part 'backup_setting_event.dart';
part 'backup_setting_state.dart';

class BackupSettingBloc extends Bloc<BackupSettingEvent, BackupSettingState> {
  BackupSettingBloc() : super(const BackupSettingInitial()) {
    on<LoadConfiguration>(_onLoadConfiguration);
    on<ApplyConfiguration>(_onApplyConfiguration);
    on<UploadConfiguration>(_onUploadConfiguration);
    on<SaveConfiguration>(_onSaveConfiguration);
  }

  Future<void> _onLoadConfiguration(
    LoadConfiguration event,
    Emitter<BackupSettingState> emit,
  ) async {
    try {
      emit(const ConfigurationLoading());
      final source = '${event.toolChain}/Script/Executor/Configuration';
      final sourceFiles =
          (await FileHelper.readDirectoryAsync(
                source: source,
                recursive: false,
              ))
              .where(
                (e) => RegExp(r'(.+)\.json$', caseSensitive: false).hasMatch(e),
              )
              .toList();

      Map<String, dynamic> configuration = {};
      for (final e in sourceFiles) {
        configuration[p.basenameWithoutExtension(e)] = converter.jsonDecode(
          await FileHelper.readFileAsync(source: e),
        );
      }

      emit(ConfigurationLoaded(configuration: configuration));
    } catch (e) {
      emit(BackupError(message: e.toString()));
    }
  }

  Future<void> _onApplyConfiguration(
    ApplyConfiguration event,
    Emitter<BackupSettingState> emit,
  ) async {
    final configuration = '${event.toolChain}/Script/Executor/Configuration';
    if (state.configuration == null) {
      return;
    }
    for (final e in state.configuration!.entries) {
      if (e.value is Map<String, dynamic> &&
          (e.value as Map<String, dynamic>).isNotEmpty) {
        final destination = '$configuration/${e.key}.json';
        final current = await FileHelper.readJsonAsync(source: destination);
        for (final key in e.value.config.keys) {
          if (current.containsKey(key)) {
            current[key] = e.value.config[key];
          }
        }
        FileHelper.writeJson(source: destination, data: current);
      }
    }
    emit(const ConfigurationApplied());
  }

  void _onUploadConfiguration(
    UploadConfiguration event,
    Emitter<BackupSettingState> emit,
  ) async {
    final source = await FileHelper.uploadFile(
      initialDirectory: event.initialDirectoryCubit.state.initialDirectory,
    );
    if (source == null) {
      return;
    }
    event.initialDirectoryCubit.setDirectoryOfFile(source: source);
    final configuration = await FileHelper.readJsonAsync(source: source);
    for (final e in configuration!.entries) {
      configuration![e.key] = e.value;
    }
    emit(ConfigurationUploaded(configuration: configuration));
  }

  void _onSaveConfiguration(
    SaveConfiguration event,
    Emitter<BackupSettingState> emit,
  ) async {
    final destination = await FileHelper.saveFile(
      suggestedName: 'configuration.json',
    );
    if (destination == null) {
      return;
    }
    if (state.configuration == null) {
      return;
    }
    FileHelper.writeJson(source: destination, data: state.configuration);
    emit(const ConfigurationSaved());
  }
}
