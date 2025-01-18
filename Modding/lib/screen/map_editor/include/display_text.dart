import 'package:flutter/material.dart';
import 'package:stroke_text/stroke_text.dart';

class DisplayText extends StatelessWidget {
  const DisplayText(
      {super.key,
      required this.displayText,
      required this.fontSize,
      required this.strokeWidth,
      this.textColor = Colors.white});

  final String displayText;

  final double fontSize;

  final double strokeWidth;

  final Color textColor;

  @override
  Widget build(BuildContext context) {
    return StrokeText(
      text: displayText,
      textStyle: TextStyle(
          color: textColor,
          fontFamily: 'PVZ2Font',
          fontSize: fontSize,
          shadows: [
            Shadow(
                color: Colors.black, offset: Offset(strokeWidth, strokeWidth))
          ]),
      strokeWidth: strokeWidth,
      maxLines: 1,
    );
  }
}
