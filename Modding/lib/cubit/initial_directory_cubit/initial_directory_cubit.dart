import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:path/path.dart' as p;

part 'initial_directory_state.dart';

class InitialDirectoryCubit extends Cubit<InitialDirectoryState> {
  InitialDirectoryCubit() : super(InitialDirectoryState.initialize());

  void setDirectoryOfFile({required String source}) {
    final path = p.dirname(source);
    emit(state.copyWith(initialDirectory: path));
  }

  void setDirectoryOfDirectory({required String source}) {
    emit(state.copyWith(initialDirectory: source));
  }

  void resetDirectory() {
    emit(state.copyWith(initialDirectory: null));
  }
}
