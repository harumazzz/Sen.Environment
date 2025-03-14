import 'package:equatable/equatable.dart';

class Dimension extends Equatable {
  const Dimension({required this.width, required this.height});
  final int width;
  final int height;

  @override
  List<Object?> get props => [width, height];
}
