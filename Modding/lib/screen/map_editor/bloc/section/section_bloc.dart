import 'dart:collection';

import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:equatable/equatable.dart';

part 'section_event.dart';
part 'section_state.dart';

class SectionBloc extends Bloc<SectionEvent, SectionState> {
  SectionBloc() : super(SectionState()) {
    on<SetSection>(_onSetSection);
    on<ExtensionToggled>(_onExtensionToggled);
    on<SectionMinizeToggled>(_onSectionMinizeToggled);
  }

  void _onSetSection(SetSection event, Emitter<SectionState> emit) {
    emit(state.copyWith(section: event.section));
  }

  void _onSectionMinizeToggled(
    SectionMinizeToggled event,
    Emitter<SectionState> emit,
  ) {
    final sectionStatus = state.sectionMinize[event.type];
    assert(sectionStatus != null,
        'failed get tool stats by tool type ${event.type}');
    final newState = state.copyWith();
    final type = event.type;
    newState.sectionMinize[type] = event.minize ?? !sectionStatus!;
    emit(newState);
  }


  void _onExtensionToggled(
    ExtensionToggled event,
    Emitter<SectionState> emit,
  ) {
    final extensionStatus = state.extension[event.type];
    assert(extensionStatus != null,
        'failed get tool stats by tool type ${event.type}');
    final newState = state.copyWith();
    final type = event.type;
    switch (type) {
      case ExtensionType.history:
      case ExtensionType.layer:
        {
          newState.extension[ExtensionType.palette] = false;
          newState.extension[ExtensionType.setting] = false;
        }
      case ExtensionType.palette:
      case ExtensionType.setting:
        {
          newState.extension[ExtensionType.history] = false;
          newState.extension[ExtensionType.layer] = false;
        }
    }
    newState.extension[type] = event.enabled ?? !extensionStatus!;
    emit(newState);
  }

  bool onExtensionEnabled(ExtensionType type) {
    final extensionStatus = state.extension[type];
    assert(extensionStatus != null, 'failed get tool stats by tool type $type');
    return extensionStatus!;
  }
}
