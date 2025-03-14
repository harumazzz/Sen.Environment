import 'package:equatable/equatable.dart';

class Range<T extends Object> extends Equatable {
  const Range({required this.begin, required this.end});
  final T begin;
  final T end;

  @override
  List<Object> get props => [begin, end];
}

final class MapConst extends Equatable {
  // static Offset debugOffset = Offset.zero;

  static const int topAdditionalPos = 60;

  static const int leftAdditonalPos = 65;

  static const double pi = 3.1415926535897932;

  static const double safeDesktopAdditionalWidth = 2000;

  static const double safeDesktopAdditionalHeight = 1000;

  static const double safeMobileAdditionalWidth = 500;

  static const double safeMobileAdditionalHeight = 2000;

  static const int minBoundingWidth = 800;

  static const int minBoundingHeight = 600;

  static const int scaleResolution = 600;

  static const int imageResolution = 1536;

  static const int animationResolution = 1200;

  static const int animationSizeCenter = 390;

  static const intMaxValue = 2147483647;

  static const intMinValue = -2147483647;

  static const double doubleMaxValue = double.maxFinite;

  static const double doubleMinValue = -double.maxFinite;

  //static const layerRange = Range<int>(begin: -10, end: 10);

  static const parallax = Range<int>(begin: -4, end: 10);

  static const Map<int, double> baseParallaxOffsets = {
    -4: -0.4,
    -3: -0.3,
    -2: -0.2,
    -1: -0.1,
    0: 0.0,
    1: 0.1,
    2: 0.2,
    3: 0.3,
    4: 0.4,
    5: 0.5,
    6: 0.6,
    7: 0.7,
    8: 0.8,
    9: 0.9,
    10: 1.0,
  };

  @override
  List<Object?> get props => throw UnimplementedError();
}
