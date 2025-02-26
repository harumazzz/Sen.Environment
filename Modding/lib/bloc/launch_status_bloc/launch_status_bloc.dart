import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:sen/bridge/client.dart';
import 'package:sen/bridge/launcher.dart';

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
    emit(const LaunchStatusStart());
    emit(const LaunchStatusLoading());
    await Launcher.launch(event.client, event.arguments);
  }

  void _finish(
    LaunchStatusComplete event,
    Emitter<LaunchStatusState> emit,
  ) {
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
