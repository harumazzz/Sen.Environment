import 'package:equatable/equatable.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../models/config.dart';

part 'suggestion_event.dart';
part 'suggestion_state.dart';

class SuggestionBloc extends Bloc<SuggestionEvent, SuggestionState> {
  SuggestionBloc()
    : super(
        const SuggestionState(
          plantList: [],
          upgradeList: [],
          narrationList: [],
          eventNameList: [],
        ),
      ) {
    on<InitailizeSuggestionList>(_initialize);
    on<UpdateEventNameList>(_updateEventName);
  }

  _initialize(InitailizeSuggestionList event, Emitter<SuggestionState> emit) {
    final configModel = event.configModel;
    emit(
      state.copyWith(
        plantList: configModel.resource.plant.keys,
        upgradeList: configModel.resource.upgrade.keys,
        narrationList: configModel.resource.narration,
      ),
    );
  }

  _updateEventName(UpdateEventNameList event, Emitter<SuggestionState> emit) {
    emit(state.copyWith(eventNameList: event.eventNameList));
  }

  Iterable<String> getSuggestion(
    Iterable<String> list,
    String query, {
    String? expect,
  }) {
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
