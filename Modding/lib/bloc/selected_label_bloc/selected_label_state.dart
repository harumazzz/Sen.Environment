part of 'selected_label_bloc.dart';

final class SelectedLabelState extends Equatable {
  final String label;

  const SelectedLabelState({
    required this.label,
  });

  @override
  List<Object> get props => [label];
}
