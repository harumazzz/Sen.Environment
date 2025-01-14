import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';

part 'selected_image_event.dart';
part 'selected_image_state.dart';

class SelectedImageBloc extends Bloc<SelectedImageEvent, SelectedImageState> {
  SelectedImageBloc() : super(const SelectedImageState(value: [])) {
    on<SelectedImageResetEvent>(_onReset);
    on<SelectedImageToggleEvent>(_onToggle);
    on<SelectedImageAllocateEvent>(_onAllocate);
    on<SelectedImageEnableAllEvent>(_onEnableAll);
    on<SelectedImageDisableAllEvent>(_onDisableAll);
  }

  void _onReset(
    SelectedImageResetEvent event,
    Emitter<SelectedImageState> emit,
  ) {
    emit(const SelectedImageState(value: []));
  }

  void _onToggle(
    SelectedImageToggleEvent event,
    Emitter<SelectedImageState> emit,
  ) {
    final currentState = state.value;
    final updatedvalue = [
      for (var i = 0; i < currentState.length; i++)
        if (i == event.index) !currentState[i] else currentState[i],
    ];
    emit(SelectedImageState(value: updatedvalue));
  }

  void _onAllocate(
    SelectedImageAllocateEvent event,
    Emitter<SelectedImageState> emit,
  ) {
    final newvalue = List.generate(event.size, (index) => true);
    emit(SelectedImageState(value: newvalue));
  }

  void _onEnableAll(
    SelectedImageEnableAllEvent event,
    Emitter<SelectedImageState> emit,
  ) {
    final newvalue = List.generate(state.value.length, (index) => true);
    emit(SelectedImageState(value: newvalue));
  }

  void _onDisableAll(
    SelectedImageDisableAllEvent event,
    Emitter<SelectedImageState> emit,
  ) {
    final newvalue = List.generate(state.value.length, (index) => false);
    emit(SelectedImageState(value: newvalue));
  }
}
