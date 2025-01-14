part of 'selected_sprite_bloc.dart';

class SelectedSpriteState extends Equatable {
  final List<bool> value;

  const SelectedSpriteState({
    required this.value,
  });

  @override
  List<Object?> get props => [value];

  SelectedSpriteState copyWith({
    List<bool>? value,
  }) {
    return SelectedSpriteState(
      value: value ?? this.value,
    );
  }
}
