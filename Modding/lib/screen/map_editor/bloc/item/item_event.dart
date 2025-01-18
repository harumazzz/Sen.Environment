part of 'item_bloc.dart';

sealed class ItemEvent extends Equatable {
  const ItemEvent();

  @override
  List<Object> get props => [];
}

final class ItemStoreClear extends ItemEvent {
  const ItemStoreClear();

  @override
  List<Object> get props => [];
}

/*
final class ItemStoreForceUpdated extends ItemEvent {
  const ItemStoreForceUpdated({required this.context});

  final BuildContext context;

  @override
  List<Object> get props => [];
}
*/

final class ItemStoreUpdated extends ItemEvent {
  const ItemStoreUpdated();

  @override
  List<Object> get props => [];
}
