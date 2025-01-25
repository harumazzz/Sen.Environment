import 'dart:async';

import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';

part 'autosave_event.dart';
part 'autosave_state.dart';

class AutosaveBloc extends Bloc<AutosaveEvent, AutosaveState> {
  late Timer _timer;

  AutosaveBloc() : super(AutosaveInitial()) {
    _timer = Timer(const Duration(seconds: 5), () {
      add(const AutosaveEvent());
    });
    on<AutosaveEvent>(_onAutosaveEvent);
  }

  void _onAutosaveEvent(
    AutosaveEvent event,
    Emitter<AutosaveState> emit,
  ) {}

  @override
  Future<void> close() {
    _timer.cancel();
    return super.close();
  }
}
