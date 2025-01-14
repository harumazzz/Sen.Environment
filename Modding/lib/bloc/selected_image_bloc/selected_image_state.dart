part of 'selected_image_bloc.dart';

class SelectedImageState extends Equatable {
  final List<bool> value;

  const SelectedImageState({
    required this.value,
  });

  @override
  List<Object?> get props => [value];

  SelectedImageState copyWith({
    List<bool>? value,
  }) {
    return SelectedImageState(
      value: value ?? this.value,
    );
  }
}
