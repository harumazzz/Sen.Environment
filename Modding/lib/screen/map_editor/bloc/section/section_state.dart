part of 'section_bloc.dart';

enum SectionType { select, image, animation, event }

//extension selection
enum ExtensionType { layer, history, setting, palette }

enum NavigationType { tool, item, option }

final class SectionState extends Equatable {
  SectionState({
    this.section = SectionType.select,
    this.navigator = NavigationType.tool,
  }) : sectionMinize = HashMap.of({
         SectionType.select: false,
         SectionType.image: false,
         SectionType.animation: false,
         SectionType.event: false,
       }),
       extension = HashMap.of({
         ExtensionType.layer: false,
         ExtensionType.history: false,
         ExtensionType.setting: false,
         ExtensionType.palette: false,
       });

  final NavigationType navigator;

  final SectionType section;

  final HashMap<SectionType, bool> sectionMinize;

  final HashMap<ExtensionType, bool> extension;

  SectionState copyWith({
    SectionType? section,
    NavigationType? navigator,
    HashMap<SectionType, bool>? sectionMinize,
    HashMap<ExtensionType, bool>? extension,
  }) {
    final newState = SectionState(
      section: section ?? this.section,
      navigator: navigator ?? this.navigator,
    );
    final dataExtension = extension ?? this.extension;
    for (final e in dataExtension.entries) {
      newState.extension[e.key] = e.value;
    }
    final dataSection = sectionMinize ?? this.sectionMinize;
    for (final e in dataSection.entries) {
      newState.sectionMinize[e.key] = e.value;
    }
    return newState;
  }

  @override
  List<Object?> get props => [section, navigator, sectionMinize, extension];
}
