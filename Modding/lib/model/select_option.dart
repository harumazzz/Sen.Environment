import 'package:equatable/equatable.dart';

class SelectOption extends Equatable {
  final String option;

  final String title;

  const SelectOption({
    required this.option,
    required this.title,
  });

  @override
  List<Object?> get props => [option, title];

  @override
  String toString() {
    return title;
  }
}
