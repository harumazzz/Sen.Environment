part of 'section_bloc.dart';

sealed class SectionEvent extends Equatable {
  const SectionEvent();

  @override
  List<Object> get props => [];
}

class SetSection extends SectionEvent {
  const SetSection({required this.section});

  final SectionType section;

  @override
  List<Object> get props => [section];
}

class ExtensionEnabled extends SectionEvent {

  const ExtensionEnabled({required this.type});

  final ExtensionType type;

  @override
  List<Object> get props => [type];
}

class ExtensionToggled extends SectionEvent {

  const ExtensionToggled({required this.type, this.enabled});

  final ExtensionType type;
  final bool? enabled;

  @override
  List<Object> get props => [type];
}

class SectionMinizeToggled extends SectionEvent {

  const SectionMinizeToggled({required this.type, this.minize});

  final SectionType type;
  final bool? minize;

  @override
  List<Object> get props => [type];
}