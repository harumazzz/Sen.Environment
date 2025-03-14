part of 'selected_sprite_bloc.dart';

class SelectedSpriteState extends Equatable {
  const SelectedSpriteState({required this.value});
  final List<bool> value;

  @override
  List<Object?> get props => [value];

  SelectedSpriteState copyWith({List<bool>? value}) {
    return SelectedSpriteState(value: value ?? this.value);
  }
}
