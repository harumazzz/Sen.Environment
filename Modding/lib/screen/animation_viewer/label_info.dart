import 'package:equatable/equatable.dart';

class LabelInfo extends Equatable {
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

  @override
  List<Object> get props => [startIndex, endIndex];
}
