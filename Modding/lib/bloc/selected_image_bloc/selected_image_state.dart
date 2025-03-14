part of 'selected_image_bloc.dart';

class SelectedImageState extends Equatable {
  const SelectedImageState({required this.value});
  final List<bool> value;

  @override
  List<Object?> get props => [value];

  SelectedImageState copyWith({List<bool>? value}) {
    return SelectedImageState(value: value ?? this.value);
  }
}
