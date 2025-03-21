import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';
import 'package:collection/collection.dart';

part 'argument_event.dart';
part 'argument_state.dart';

class ArgumentBloc extends Bloc<ArgumentEvent, ArgumentState> {
  ArgumentBloc() : super(const ArgumentInitial(value: [])) {
    on<AddArgument>(_addArgument);
    on<ClearArgument>(_clearArgument);
    on<RemoveArgument>(_removeArgument);
  }

  void _addArgument(AddArgument event, Emitter<ArgumentState> emit) {
    emit(ArgumentSubmittedState(value: [...state.value, ...event.value]));
  }

  void _clearArgument(ClearArgument event, Emitter<ArgumentState> emit) {
    emit(const ArgumentClearState(value: []));
  }

  void _removeArgument(RemoveArgument event, Emitter<ArgumentState> emit) {
    emit(
      ArgumentSubmittedState(
        value: [...state.value.whereIndexed((i, _) => i != event.index)],
      ),
    );
  }
}
