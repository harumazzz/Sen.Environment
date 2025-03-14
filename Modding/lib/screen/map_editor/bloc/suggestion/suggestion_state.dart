part of 'suggestion_bloc.dart';

final class SuggestionState extends Equatable {
  const SuggestionState({
    required this.plantList,
    required this.upgradeList,
    required this.narrationList,
    required this.eventNameList,
    /*
    required this.matchPlantList,
    required this.matchNarrationList,
    required this.matchUpgradeList,
    required this.matchEventNameList,
    */
  });

  final Iterable<String> plantList;

  final Iterable<String> upgradeList;

  final Iterable<String> narrationList;

  final Iterable<String> eventNameList;
  /*

  final Iterable<String> matchPlantList;

  final Iterable<String> matchNarrationList;

  final Iterable<String> matchUpgradeList;

  final Iterable<String> matchEventNameList;
  */

  SuggestionState copyWith({
    Iterable<String>? plantList,
    Iterable<String>? upgradeList,
    Iterable<String>? narrationList,
    Iterable<String>? eventNameList,
  }) {
    return SuggestionState(
      plantList: plantList ?? this.plantList,
      upgradeList: upgradeList ?? this.upgradeList,
      narrationList: narrationList ?? this.narrationList,
      eventNameList: eventNameList ?? this.eventNameList,
      /*
      matchPlantList: matchPlantList ?? const [],
      matchNarrationList: matchNarrationList ?? const [],
      matchUpgradeList: matchUpgradeList ?? const [],
      matchEventNameList: matchEventNameList ?? const [],
      */
    );
  }

  @override
  List<Object> get props => [
    plantList,
    upgradeList,
    narrationList,
    eventNameList,
    /*
        matchPlantList,
        matchNarrationList,
        matchUpgradeList,
        matchEventNameList
        */
  ];
}
