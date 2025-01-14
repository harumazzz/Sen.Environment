class LabelInfo {
  final int startIndex;
  final int endIndex;

  const LabelInfo({
    required this.startIndex,
    required this.endIndex,
  });

  LabelInfo copyWith({
    int? startIndex,
    int? endIndex,
  }) {
    return LabelInfo(
      startIndex: startIndex ?? this.startIndex,
      endIndex: endIndex ?? this.endIndex,
    );
  }
}
