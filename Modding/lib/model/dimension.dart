import 'package:equatable/equatable.dart';

class Dimension extends Equatable {
  final int width;
  final int height;

  const Dimension({
    required this.width,
    required this.height,
  });

  @override
  List<Object?> get props => [width, height];
}
