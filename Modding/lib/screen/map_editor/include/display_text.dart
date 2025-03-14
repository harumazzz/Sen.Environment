import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';

class DisplayText extends StatelessWidget {
  const DisplayText({
    super.key,
    required this.displayText,
    required this.fontSize,
    required this.strokeWidth,
    this.textColor = Colors.white,
  });

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
        shadows: [Shadow(offset: Offset(strokeWidth, strokeWidth))],
      ),
      strokeWidth: strokeWidth,
      maxLines: 1,
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('displayText', displayText));
    properties.add(DoubleProperty('fontSize', fontSize));
    properties.add(DoubleProperty('strokeWidth', strokeWidth));
    properties.add(ColorProperty('textColor', textColor));
  }
}

// StrokeText: https://github.com/MohamedAbd0/stroke_text v0.0.3
class StrokeText extends StatelessWidget {
  const StrokeText({
    super.key,
    required this.text,
    this.strokeWidth = 1,
    this.strokeColor = Colors.black,
    this.textColor = Colors.white,
    this.textStyle,
    this.textAlign,
    this.textDirection,
    this.textScaler,
    this.overflow,
    this.maxLines,
  });
  final String text;
  final double strokeWidth;
  final Color textColor;
  final Color strokeColor;
  final TextStyle? textStyle;
  final TextAlign? textAlign;
  final TextDirection? textDirection;
  final TextScaler? textScaler;
  final TextOverflow? overflow;
  final int? maxLines;

  @override
  Widget build(BuildContext context) {
    return Stack(
      children: [
        Text(
          text,
          style: TextStyle(
            foreground:
                Paint()
                  ..style = PaintingStyle.stroke
                  ..strokeWidth = strokeWidth
                  ..color = strokeColor,
          ).merge(textStyle),
          textAlign: textAlign,
          textDirection: textDirection,
          textScaler: textScaler,
          overflow: overflow,
          maxLines: maxLines,
        ),
        Text(
          text,
          style: TextStyle(color: textColor).merge(textStyle),
          textDirection: textDirection,
          textScaler: textScaler,
          overflow: overflow,
          maxLines: maxLines,
        ),
      ],
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('text', text));
    properties.add(DoubleProperty('strokeWidth', strokeWidth));
    properties.add(ColorProperty('textColor', textColor));
    properties.add(ColorProperty('strokeColor', strokeColor));
    properties.add(DiagnosticsProperty<TextStyle?>('textStyle', textStyle));
    properties.add(EnumProperty<TextAlign?>('textAlign', textAlign));
    properties.add(
      EnumProperty<TextDirection?>('textDirection', textDirection),
    );
    properties.add(DiagnosticsProperty<TextScaler?>('textScaler', textScaler));
    properties.add(EnumProperty<TextOverflow?>('overflow', overflow));
    properties.add(IntProperty('maxLines', maxLines));
  }
}
