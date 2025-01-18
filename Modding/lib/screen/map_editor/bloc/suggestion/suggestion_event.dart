part of 'suggestion_bloc.dart';

sealed class SuggestionEvent extends Equatable {
  const SuggestionEvent();

  @override
  List<Object> get props => [];
}

final class InitailizeSuggestionList extends SuggestionEvent {
  const InitailizeSuggestionList({required this.configModel});

  final ConfigModel configModel;

  @override
  List<Object> get props => [configModel];
}

final class UpdateEventNameList extends SuggestionEvent {
  const UpdateEventNameList({required this.eventNameList});

  final Iterable<String> eventNameList;

  @override
  List<Object> get props => [eventNameList];
}

/*
final class SuggestionPlantListEvent extends SuggestionEvent {
  const SuggestionPlantListEvent({required this.query});

  final String query;

  @override
  List<Object> get props => [query];
}

final class SuggestionNarrationListEvent extends SuggestionEvent {
  const SuggestionNarrationListEvent({required this.query});

  final String query;

  @override
  List<Object> get props => [query];
}

final class SuggestionUpgradeListEvent extends SuggestionEvent {
  const SuggestionUpgradeListEvent({required this.query});

  final String query;

  @override
  List<Object> get props => [query];
}

final class SuggestionEventNameListEvent extends SuggestionEvent {
  const SuggestionEventNameListEvent({required this.query});

  final String query;

  @override
  List<Object> get props => [query];
}
*/
