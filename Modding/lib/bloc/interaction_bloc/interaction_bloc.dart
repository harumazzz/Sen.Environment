import 'dart:async';

import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';

part 'interaction_event.dart';
part 'interaction_state.dart';

class InteractionBloc extends Bloc<InteractionEvent, InteractionState> {
  InteractionBloc() : super(const InteractionInitial()) {
    on<BooleanInputEvent>(_inputBoolean);
    on<StringInputEvent>(_inputString);
    on<EnumerationSelectEvent>(_selectEnumeration);
    on<StringInputCompleteEvent>(_submitString);
    on<BooleanInputCompleteEvent>(_submitBoolean);
    on<EnumerationSelectCompleteEvent>(_submitEnumeration);
    on<PickLocalStorage>(_pickLocalStorage);
  }

  void _inputBoolean(BooleanInputEvent event, Emitter<InteractionState> emit) {
    emit(BooleanInputState(completer: event.completer));
  }

  void _pickLocalStorage(
    PickLocalStorage event,
    Emitter<InteractionState> emit,
  ) {
    emit(const BlockInteractionState());
  }

  void _inputString(StringInputEvent event, Emitter<InteractionState> emit) {
    emit(StringInputState(completer: event.completer));
  }

  void _submitString(
    StringInputCompleteEvent event,
    Emitter<InteractionState> emit,
  ) {
    assert(state is StringInputState);
    (state as StringInputState).completer.complete(event.value);
    emit(const BlockInteractionState());
  }

  void _submitEnumeration(
    EnumerationSelectCompleteEvent event,
    Emitter<InteractionState> emit,
  ) {
    assert(state is EnumerationSelectState);
    (state as EnumerationSelectState).completer.complete(event.value);
    emit(const BlockInteractionState());
  }

  void _submitBoolean(
    BooleanInputCompleteEvent event,
    Emitter<InteractionState> emit,
  ) {
    assert(state is BooleanInputState);
    var result = null as String?;
    if (event.value != null && event.value!) {
      result = '1';
    }
    (state as BooleanInputState).completer.complete(result ?? '0');
    emit(const BlockInteractionState());
  }

  void _selectEnumeration(
    EnumerationSelectEvent event,
    Emitter<InteractionState> emit,
  ) {
    emit(EnumerationSelectState(completer: event.completer));
    assert(state is EnumerationSelectState);
  }
}
