part of 'selected_bloc.dart';

sealed class SelectedEvent extends Equatable {
  const SelectedEvent();

  @override
  List<Object> get props => [];
}

final class OnSelectUpdated extends SelectedEvent {
  const OnSelectUpdated({required this.onSelect});

  final String onSelect;

  @override
  List<Object> get props => [onSelect];
}

final class AddSelectionEvent extends SelectedEvent {
  const AddSelectionEvent({required this.id, this.toggle = false});

  final String id;

  final bool toggle;

  @override
  List<Object> get props => [id, toggle];
}

final class SetSelectionEvent extends SelectedEvent {
  const SetSelectionEvent({required this.id});

  final String id;

  @override
  List<Object> get props => [id];
}

final class ClearSelectedList extends SelectedEvent {
  const ClearSelectedList();

  @override
  List<Object> get props => [];
}

final class SelectedListUpdated extends SelectedEvent {
  const SelectedListUpdated({required this.selectedList});

  final Iterable<String> selectedList;

  @override
  List<Object> get props => [selectedList];
}

final class CopySelectedList extends SelectedEvent {
  const CopySelectedList({required this.isCut});

  final bool isCut;

  @override
  List<Object> get props => [isCut];
}

final class ClearCopyList extends SelectedEvent {
  const ClearCopyList();

  // final Iterable<String>? selectedList;

  @override
  List<Object> get props => [];
}

final class SetOnSelectedNull extends SelectedEvent {
  const SetOnSelectedNull();

  @override
  List<Object> get props => [];
}

final class SetHoverSelected extends SelectedEvent {
  const SetHoverSelected({required this.selected});

  final bool selected;

  @override
  List<Object> get props => [selected];
}
