part of 'selected_label_bloc.dart';

@immutable
sealed class SelectedLabelEvent extends Equatable {
  const SelectedLabelEvent();

  @override
  List<Object?> get props => [];
}

class SetLabelEvent extends SelectedLabelEvent {
  const SetLabelEvent({required this.newLabel});
  final String newLabel;

  @override
  List<Object?> get props => [newLabel];
}

class ResetLabelEvent extends SelectedLabelEvent {
  const ResetLabelEvent();
}
