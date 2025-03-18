import 'package:equatable/equatable.dart';

class SelectOption extends Equatable {
  const SelectOption({required this.option, required this.title});
  final String option;

  final String title;

  @override
  List<Object?> get props => [option, title];

  String get regularOption {
    return '$option. $title';
  }

  @override
  String toString() {
    return title;
  }
}
