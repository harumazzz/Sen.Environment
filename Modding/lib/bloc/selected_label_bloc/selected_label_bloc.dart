import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';

part 'selected_label_event.dart';
part 'selected_label_state.dart';

class SelectedLabelBloc extends Bloc<SelectedLabelEvent, SelectedLabelState> {
  SelectedLabelBloc() : super(const SelectedLabelState(label: 'main')) {
    on<SetLabelEvent>(_onSetLabel);
    on<ResetLabelEvent>(_onResetLabel);
  }

  void _onSetLabel(SetLabelEvent event, Emitter<SelectedLabelState> emit) {
    emit(SelectedLabelState(label: event.newLabel));
  }

  void _onResetLabel(ResetLabelEvent event, Emitter<SelectedLabelState> emit) {
    emit(const SelectedLabelState(label: 'main'));
  }
}
