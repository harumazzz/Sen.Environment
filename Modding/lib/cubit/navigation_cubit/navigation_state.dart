part of 'navigation_cubit.dart';

@immutable
sealed class NavigationState extends Equatable {
  final int selectedIndex;

  const NavigationState({
    required this.selectedIndex,
  });

  @override
  List<Object?> get props => [selectedIndex];
}

final class NavigationInitial extends NavigationState {
  const NavigationInitial({
    required super.selectedIndex,
  });
}

final class NavigationChange extends NavigationState {
  const NavigationChange({
    required super.selectedIndex,
  });
}
