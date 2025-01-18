part of 'item_bloc.dart';

typedef ItemStore = Map<String, ItemProfile>;

final class ItemState extends Equatable {
  const ItemState({required this.itemStore});

  final ItemStore itemStore;

  @override
  List<Object?> get props => [itemStore];
}
