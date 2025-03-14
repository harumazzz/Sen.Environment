import 'package:equatable/equatable.dart';
import 'package:flutter_bloc/flutter_bloc.dart';

part 'selected_event.dart';
part 'selected_state.dart';

class SelectedBloc extends Bloc<SelectedEvent, SelectedState> {
  SelectedBloc() : super(SelectedState.initailize()) {
    on<OnSelectUpdated>(_onSelectedUpdate);
    on<AddSelectionEvent>(_addSelection);
    on<SetSelectionEvent>(_setSelection);
    on<ClearSelectedList>(_clearSelectedList);
    on<SelectedListUpdated>(_selectedListUpdate);
    on<SetOnSelectedNull>(_setNull);
    on<CopySelectedList>(_copySelected);
    on<ClearCopyList>(_clearSelected);
    on<SetHoverSelected>(_setHoverSelected);
  }

  void _onSelectedUpdate(OnSelectUpdated event, Emitter<SelectedState> emit) {
    emit(state.copyWith(onSelect: event.onSelect));
  }

  void _addSelection(AddSelectionEvent event, Emitter<SelectedState> emit) {
    final id = event.id;
    if (!state.selectedList.contains(id)) {
      final newState = state.copyWith(
        selectedList: [...state.selectedList, id],
      );
      emit(newState);
    } else {
      if (event.toggle) {
        final newState = state.copyWith(
          selectedList: state.selectedList.toList()..remove(id),
        );
        emit(newState);
      }
    }
  }

  void _setSelection(SetSelectionEvent event, Emitter<SelectedState> emit) {
    emit(state.copyWith(selectedList: [event.id]));
  }

  void _clearSelectedList(
    ClearSelectedList event,
    Emitter<SelectedState> emit,
  ) {
    emit(SelectedState(selectedList: const [], copyList: state.copyList));
  }

  void _selectedListUpdate(
    SelectedListUpdated event,
    Emitter<SelectedState> emit,
  ) {
    emit(state.copyWith(selectedList: event.selectedList));
  }

  void _copySelected(CopySelectedList event, Emitter<SelectedState> emit) {
    final newState = state.copyWith(
      selectedList: state.selectedList,
      copyList:
          event.isCut
              ? [...state.selectedList, 'paste_cut']
              : state.selectedList,
    );

    emit(newState);
  }

  void _clearSelected(ClearCopyList event, Emitter<SelectedState> emit) {
    emit(state.copyWith(selectedList: state.selectedList, copyList: []));
  }

  void _setNull(SetOnSelectedNull event, Emitter<SelectedState> emit) {
    emit(
      SelectedState(
        selectedList: state.selectedList,
        copyList: state.copyList,
        hoverSelect: state.hoverSelect,
      ),
    );
  }

  void _setHoverSelected(SetHoverSelected event, Emitter<SelectedState> emit) {
    emit(state.copyWith(hoverSelect: event.selected));
  }
}
