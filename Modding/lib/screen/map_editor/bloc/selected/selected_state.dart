part of 'selected_bloc.dart';

final class SelectedState extends Equatable {
  const SelectedState({
    required this.selectedList,
    required this.copyList,
    this.hoverSelect = false,
    this.onSelect,
  });

  final String? onSelect;

  final bool hoverSelect;

  final Iterable<String> selectedList;

  final Iterable<String> copyList;

  factory SelectedState.initailize() {
    return const SelectedState(copyList: [], selectedList: [], hoverSelect: false, onSelect: null);
  }

  SelectedState copyWith(
      {String? onSelect,
      bool? hoverSelect,
      Iterable<String>? selectedList,
      Iterable<String>? copyList}) {
    return SelectedState(
        onSelect: onSelect ?? this.onSelect,
        hoverSelect: hoverSelect ?? this.hoverSelect,
        selectedList: [...(selectedList ?? this.selectedList)],
        copyList: [...(copyList ?? this.copyList)]);
  }

  @override
  List<Object?> get props => [onSelect, hoverSelect, selectedList, copyList];
}
