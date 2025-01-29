import 'dart:collection';
import 'dart:typed_data';
import 'package:equatable/equatable.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

part 'init_event.dart';
part 'init_state.dart';

class InitBloc extends Bloc<InitEvent, InitState> {
  InitBloc() : super(InitState.initailize()) {
    on<SetStatusEvent>(_setStatus);
    on<ShowSnackBarEvent>(_showSnackBar);
    on<ShowAlertDialog>(_showAlertDialog);
    on<CreateTakeShootFunction>(_createTakeShoot);
  }

  _setStatus(SetStatusEvent event, Emitter<InitState> emit) {
    emit(state.copyWith(status: InitailizeStatus.success));
  }

  _showSnackBar(ShowSnackBarEvent event, Emitter<InitState> emit) {
    emit(state.copyWith(text: event.text));
    emit(state.copyWith(text: 'null'));
  }

  _showAlertDialog(ShowAlertDialog event, Emitter<InitState> emit) {
    final newState = state.copyWith();
    newState.alertDialogEnable[event.type] = event.enable;
    emit(newState);
  }

  _createTakeShoot(CreateTakeShootFunction event, Emitter<InitState> emit) {
    emit(state.copyWith(takeShoot: event.takeShoot));
  }
}
