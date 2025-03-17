import 'package:bloc/bloc.dart';
import 'package:dio/dio.dart' as dio;
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';
import '../../constant/build_distribution.dart';
import '../../cubit/settings_cubit/settings_cubit.dart';
import '../../service/file_helper.dart';

part 'miscellaneous_task_event.dart';
part 'miscellaneous_task_state.dart';

class MiscellaneousTaskBloc
    extends Bloc<MiscellaneousTaskEvent, MiscellaneousTaskState> {
  MiscellaneousTaskBloc() : super(const MiscellaneousTaskInitial()) {
    on<BackupConfigurationRequested>(_onBackupConfiguration);
    on<DownloadScriptRequested>(_onDownloadScript);
  }

  Future<void> _onBackupConfiguration(
    BackupConfigurationRequested event,
    Emitter<MiscellaneousTaskState> emit,
  ) async {
    emit(const BackupInProgress());
    await Future.delayed(const Duration(seconds: 2));
    emit(const BackupCompleted());
  }

  Future<void> _onDownloadScript(
    DownloadScriptRequested event,
    Emitter<MiscellaneousTaskState> emit,
  ) async {
    emit(const DownloadingScript());
    try {
      final destination =
          '${await FileHelper.getWorkingDirectory()}/CDN ${BuildDistribution.kVersion}';
      if (!FileHelper.isDirectory(destination)) {
        FileHelper.createDirectory(destination);
      }
      final source = '$destination/Script.zip';
      final service = dio.Dio();
      await service.download(
        'https://github.com/harumazzz/Sen.Environment/releases/download/script/Script.zip',
        source,
      );
      await FileHelper.unzipFile(source, '$destination/Script');
      await Future.delayed(const Duration(milliseconds: 300));
      await FileHelper.removeFileAsync(source);
      await event.settingsCubit.setToolChain(destination);
      await event.settingsCubit.setIsValid(isValid: true);
      emit(const ScriptDownloaded());
    } catch (e) {
      emit(ScriptDownloadFailed(error: e.toString()));
    }
  }
}
