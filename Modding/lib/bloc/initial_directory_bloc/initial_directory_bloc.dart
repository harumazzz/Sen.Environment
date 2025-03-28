import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';
import 'package:path/path.dart' as p;

part 'initial_directory_event.dart';
part 'initial_directory_state.dart';

class InitialDirectoryBloc
    extends Bloc<InitialDirectoryEvent, InitialDirectoryState> {
  InitialDirectoryBloc() : super(const InitialDirectoryEmpty()) {
    on<SetDirectoryOfFile>(_onSetDirectoryOfFile);
    on<SetDirectoryOfDirectory>(_onSetDirectoryOfDirectory);
    on<ResetDirectory>(_onResetDirectory);
  }

  void _onSetDirectoryOfFile(
    SetDirectoryOfFile event,
    Emitter<InitialDirectoryState> emit,
  ) {
    emit(const InitialDirectoryLoading());
    final path = p.dirname(event.source);
    emit(InitialDirectoryLoaded(initialDirectory: path));
  }

  void _onSetDirectoryOfDirectory(
    SetDirectoryOfDirectory event,
    Emitter<InitialDirectoryState> emit,
  ) {
    emit(const InitialDirectoryLoading());
    emit(InitialDirectoryLoaded(initialDirectory: event.source));
  }

  void _onResetDirectory(
    ResetDirectory event,
    Emitter<InitialDirectoryState> emit,
  ) {
    emit(const InitialDirectoryEmpty());
  }
}
