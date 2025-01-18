part of 'section_bloc.dart';

enum SectionType { select, image, animation, event }

//extension selection
enum ExtensionType { layer, history, setting, palette }

final class SectionState extends Equatable {
  SectionState({this.section = SectionType.select})
      : sectionMinize = HashMap.from({
          SectionType.select: false,
          SectionType.image: false,
          SectionType.animation: false,
          SectionType.event: false,
        }),
        extension = HashMap.from({
          ExtensionType.layer: false,
          ExtensionType.history: false,
          ExtensionType.setting: false,
          ExtensionType.palette: false,
        });

  final SectionType section;

  final HashMap<SectionType, bool> sectionMinize;

  final HashMap<ExtensionType, bool> extension;

  SectionState copyWith(
      {SectionType? section,
      HashMap<SectionType, bool>? sectionMinize,
      HashMap<ExtensionType, bool>? extension}) {
    final newState = SectionState(section: section ?? this.section);
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
  List<Object?> get props => [section, sectionMinize, extension];
}
