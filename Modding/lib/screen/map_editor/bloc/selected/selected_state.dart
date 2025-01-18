part of 'selected_bloc.dart';

final class SelectedState extends Equatable {
  const SelectedState({
    required this.selectedList,
    required this.copyList,
    this.onSelect,
  });

  final String? onSelect;

  final Iterable<String> selectedList;

  final Iterable<String> copyList;

  factory SelectedState.initailize() {
    return const SelectedState(copyList: [], selectedList: [], onSelect: null);
  }

  SelectedState copyWith(
      {String? onSelect,
      Iterable<String>? selectedList,
      Iterable<String>? copyList}) {
    return SelectedState(
        onSelect: onSelect ?? this.onSelect,
        selectedList: [...(selectedList ?? this.selectedList)],
        copyList: [...(copyList ?? this.copyList)]);
  }

  @override
  List<Object?> get props => [onSelect, selectedList, copyList];
}
