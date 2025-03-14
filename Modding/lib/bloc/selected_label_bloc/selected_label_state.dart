part of 'selected_label_bloc.dart';

final class SelectedLabelState extends Equatable {
  const SelectedLabelState({required this.label});
  final String label;

  @override
  List<Object> get props => [label];
}
