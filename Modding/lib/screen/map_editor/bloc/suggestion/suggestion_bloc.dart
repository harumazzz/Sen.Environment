import 'package:equatable/equatable.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/screen/map_editor/models/config.dart';

part 'suggestion_event.dart';
part 'suggestion_state.dart';

class SuggestionBloc extends Bloc<SuggestionEvent, SuggestionState> {
  SuggestionBloc()
      : super(const SuggestionState(
          plantList: [],
          upgradeList: [],
          narrationList: [],
          eventNameList: [],
          /*
          matchPlantList: [],
          matchNarrationList: [],
          matchUpgradeList: [],
          matchEventNameList: [],
          */
        )) {
    on<InitailizeSuggestionList>(_initialize);
    on<UpdateEventNameList>(_updateEventName);
    /*
    on<SuggestionPlantListEvent>(_suggestionPlantList);
    on<SuggestionNarrationListEvent>(_suggestionNarationList);
    on<SuggestionUpgradeListEvent>(_suggestionUpgradeList);
    on<SuggestionEventNameListEvent>(_suggestionEventNameList);
    */
  }

  _initialize(InitailizeSuggestionList event, Emitter<SuggestionState> emit) {
    final configModel = event.configModel;
    emit(state.copyWith(
      plantList: configModel.resource.plant.keys,
      upgradeList: configModel.resource.upgrade.keys,
      narrationList: configModel.resource.narration,
    ));
  }

  _updateEventName(UpdateEventNameList event, Emitter<SuggestionState> emit) {
    emit(state.copyWith(eventNameList: event.eventNameList));
  }

/*
  _suggestionPlantList(
      SuggestionPlantListEvent event, Emitter<SuggestionState> emit) {
    final matches = <String>[];
    matches.addAll(state.plantList);
    matches.retainWhere(
        (s) => s.toLowerCase().contains(event.query.toLowerCase()));
    emit(state.copyWith(matchPlantList: matches));
  }

  _suggestionNarationList(
      SuggestionNarrationListEvent event, Emitter<SuggestionState> emit) {
    final matches = <String>[];
    matches.addAll(state.narrationList);
    matches.retainWhere(
        (s) => s.toLowerCase().contains(event.query.toLowerCase()));
    emit(state.copyWith(matchNarrationList: matches));
  }

  _suggestionUpgradeList(
      SuggestionUpgradeListEvent event, Emitter<SuggestionState> emit) {
    final matches = <String>[];
    matches.addAll(state.upgradeList);
    matches.retainWhere(
        (s) => s.toLowerCase().contains(event.query.toLowerCase()));
    emit(state.copyWith(matchUpgradeList: matches));
  }

  _suggestionEventNameList(
      SuggestionEventNameListEvent event, Emitter<SuggestionState> emit) {
    final matches = <String>[];
    matches.addAll(state.eventNameList);
    matches.retainWhere(
        (s) => s.toLowerCase().contains(event.query.toLowerCase()));
    emit(state.copyWith(matchEventNameList: matches));
  }

  */
  Iterable<String> getSuggestion(Iterable<String> list, String query, {String? expect}) {
    final matches = <String>[];
    matches.addAll(list);
    matches.retainWhere((s) {
      if (expect != null && expect.toLowerCase() == s) {
        return false;
      }
      return s.toLowerCase().contains(query.toLowerCase());
    });
    return matches;
  }
}
