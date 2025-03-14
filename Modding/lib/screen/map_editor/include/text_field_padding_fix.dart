import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';

class TextFieldPaddingFix extends StatelessWidget {
  const TextFieldPaddingFix({super.key, this.borderWidth = 2, this.child});

  final double borderWidth;
  final Widget? child;

  @override
  Widget build(BuildContext context) {
    return Container(
      decoration: BoxDecoration(
        borderRadius: BorderRadius.only(
          topRight: Radius.circular(borderWidth),
          bottomRight: Radius.circular(borderWidth),
        ),
      ),
      clipBehavior: Clip.antiAlias,
      alignment: Alignment.centerRight,
      margin: EdgeInsets.only(
        top: borderWidth,
        right: borderWidth,
        bottom: borderWidth,
      ),
      child: child,
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DoubleProperty('borderWidth', borderWidth));
  }
}
