import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';

part 'selected_sprite_event.dart';
part 'selected_sprite_state.dart';

class SelectedSpriteBloc
    extends Bloc<SelectedSpriteEvent, SelectedSpriteState> {
  SelectedSpriteBloc() : super(const SelectedSpriteState(value: [])) {
    on<SelectedSpriteResetEvent>(_onReset);
    on<SelectedSpriteToggleEvent>(_onToggle);
    on<SelectedSpriteAllocateEvent>(_onAllocate);
    on<SelectedSpriteEnableAllEvent>(_onEnableAll);
    on<SelectedSpriteDisableAllEvent>(_onDisableAll);
  }

  void _onReset(
    SelectedSpriteResetEvent event,
    Emitter<SelectedSpriteState> emit,
  ) {
    emit(const SelectedSpriteState(value: []));
  }

  void _onToggle(
    SelectedSpriteToggleEvent event,
    Emitter<SelectedSpriteState> emit,
  ) {
    final currentState = state.value;
    final updatedvalue = [
      for (var i = 0; i < currentState.length; i++)
        if (i == event.index) !currentState[i] else currentState[i],
    ];
    emit(SelectedSpriteState(value: updatedvalue));
  }

  void _onAllocate(
    SelectedSpriteAllocateEvent event,
    Emitter<SelectedSpriteState> emit,
  ) {
    final newvalue = List.generate(event.size, (index) => true);
    emit(SelectedSpriteState(value: newvalue));
  }

  void _onEnableAll(
    SelectedSpriteEnableAllEvent event,
    Emitter<SelectedSpriteState> emit,
  ) {
    final newvalue = List.generate(state.value.length, (index) => true);
    emit(SelectedSpriteState(value: newvalue));
  }

  void _onDisableAll(
    SelectedSpriteDisableAllEvent event,
    Emitter<SelectedSpriteState> emit,
  ) {
    final newvalue = List.generate(state.value.length, (index) => false);
    emit(SelectedSpriteState(value: newvalue));
  }
}
