import 'dart:collection';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:equatable/equatable.dart';
import '../../../../extension/platform.dart';

part 'section_event.dart';
part 'section_state.dart';

class SectionBloc extends Bloc<SectionEvent, SectionState> {
  SectionBloc() : super(SectionState()) {
    on<SetSection>(_onSetSection);
    on<SetNavigator>(_onNavigator);
    on<ExtensionToggled>(_onExtensionToggled);
    on<SectionMinizeToggled>(_onSectionMinizeToggled);
  }

  void _onSetSection(SetSection event, Emitter<SectionState> emit) {
    final newState = state.copyWith(section: event.section);
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    if (!isDesktopPlatform) {
      newState.extension[ExtensionType.palette] = false;
      newState.extension[ExtensionType.setting] = false;
      newState.extension[ExtensionType.history] = false;
      newState.extension[ExtensionType.layer] = false;
    }
    emit(newState);
  }

  void _onNavigator(SetNavigator event, Emitter<SectionState> emit) {
    emit(state.copyWith(navigator: event.navigator));
  }

  void _onSectionMinizeToggled(
    SectionMinizeToggled event,
    Emitter<SectionState> emit,
  ) {
    final sectionStatus = state.sectionMinize[event.type];
    assert(
      sectionStatus != null,
      'failed get tool stats by tool type ${event.type}',
    );
    final newState = state.copyWith();
    final type = event.type;
    newState.sectionMinize[type] = event.minize ?? !sectionStatus!;
    emit(newState);
  }

  void _onExtensionToggled(ExtensionToggled event, Emitter<SectionState> emit) {
    final extensionStatus = state.extension[event.type];
    assert(
      extensionStatus != null,
      'failed get tool stats by tool type ${event.type}',
    );

    final type = event.type;
    final view = WidgetsBinding.instance.platformDispatcher.views.first;
    final isLowScreenHeight = view.physicalSize.height < 965;
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    final newState = state.copyWith(
      section: !isDesktopPlatform ? SectionType.select : null,
    );
    if (!isLowScreenHeight && isDesktopPlatform) {
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
    } else {
      newState.extension[ExtensionType.palette] = false;
      newState.extension[ExtensionType.setting] = false;
      newState.extension[ExtensionType.history] = false;
      newState.extension[ExtensionType.layer] = false;
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
