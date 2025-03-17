import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import '../../bridge/client.dart';
import '../../bridge/launcher.dart';
import '../../cubit/settings_cubit/settings_cubit.dart';
import '../../extension/platform.dart';
import '../../service/file_helper.dart';

part 'launch_status_event.dart';
part 'launch_status_state.dart';

class LaunchStatusBloc extends Bloc<LaunchStatusEvent, LaunchStatusState> {
  LaunchStatusBloc() : super(const LaunchStatusInitial()) {
    on<LaunchStatusBegin>(_start);
    on<LaunchStatusComplete>(_finish);
    on<LaunchStatusSleep>(_sleep);
  }

  Future<void> _start(
    LaunchStatusBegin event,
    Emitter<LaunchStatusState> emit,
  ) async {
    final workingDirectory = await FileHelper.getWorkingDirectory();
    final kernel = () {
      if (CurrentPlatform.isAndroid) {
        return 'libKernel.so';
      }
      if (CurrentPlatform.isIOS) {
        return 'libKernel.dylib';
      }
      final toolChain = event.setting.state.toolChain;
      if (CurrentPlatform.isWindows) {
        return '$toolChain/Kernel.dll';
      }
      if (CurrentPlatform.isLinux) {
        return '$toolChain/Kernel.so';
      }
      if (CurrentPlatform.isMacOS) {
        return '$toolChain/Kernel.dylib';
      }
      throw Exception('Unsupported platform');
    }();
    final script = '${event.setting.state.toolChain}/Script/main.js';
    emit(const LaunchStatusStart());
    emit(const LaunchStatusLoading());
    await Launcher.launch(event.client, [
      workingDirectory,
      kernel,
      script,
      ...event.arguments,
    ]);
  }

  void _finish(LaunchStatusComplete event, Emitter<LaunchStatusState> emit) {
    emit(const LaunchStatusEnd());
  }

  Future<void> _sleep(
    LaunchStatusSleep event,
    Emitter<LaunchStatusState> emit,
  ) async {
    await Future.delayed(const Duration(milliseconds: 300));
    emit(const LaunchStatusMain());
  }
}
