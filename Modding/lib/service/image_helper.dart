import '../model/dimension.dart';
import 'file_helper.dart';
import 'package:image/image.dart';

class ImageHelper {
  const ImageHelper._();

  static void saveImage(String source, Image data) {
    FileHelper.writeBuffer(source: source, data: encodePng(data));
    return;
  }

  static void resizeImage(
    String source,
    Dimension newDimension,
    String destination,
  ) {
    final bytes = FileHelper.readBuffer(source: source);
    final image = decodeImage(bytes)!;
    final result = copyResize(
      image,
      width: newDimension.width,
      height: newDimension.height,
    );
    saveImage(destination, result);
    return;
  }

  static Image? readImage(String source) {
    return decodeImage(FileHelper.readBuffer(source: source));
  }

  static void cropImage({
    required Image original,
    required int x,
    required int y,
    required int width,
    required int height,
    required String destination,
  }) {
    final result = copyCrop(original, x: x, y: y, width: width, height: height);
    saveImage(destination, result);
    return;
  }
}
