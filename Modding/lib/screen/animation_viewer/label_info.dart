import 'package:equatable/equatable.dart';

class LabelInfo extends Equatable {
  const LabelInfo({required this.startIndex, required this.endIndex});
  final int startIndex;
  final int endIndex;

  LabelInfo copyWith({int? startIndex, int? endIndex}) {
    return LabelInfo(
      startIndex: startIndex ?? this.startIndex,
      endIndex: endIndex ?? this.endIndex,
    );
  }

  @override
  List<Object> get props => [startIndex, endIndex];
}
