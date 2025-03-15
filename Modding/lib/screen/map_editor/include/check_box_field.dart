import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import '../../../extension/platform.dart';

class CheckBoxField extends StatelessWidget {
  const CheckBoxField({
    super.key,
    required this.label,
    required this.value,
    required this.onChanged,
    this.width,
    this.height,
    this.margin,
    this.underline = true,
  });

  final String label;

  final bool value;

  final bool underline;

  final double? width;

  final double? height;

  final EdgeInsetsGeometry? margin;

  final void Function({required bool? value})? onChanged;

  @override
  Widget build(BuildContext context) {
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    final colorScheme = Theme.of(context).colorScheme;
    final activeColor =
        !isDesktopPlatform && Theme.of(context).brightness == Brightness.light
            ? colorScheme.inversePrimary
            : colorScheme.primaryFixedDim;
    return Container(
      width: width ?? double.infinity,
      height: height,
      margin: margin ?? const EdgeInsets.fromLTRB(12, 16, 20, 16),
      decoration:
          underline
              ? BoxDecoration(
                border: Border(
                  bottom: BorderSide(
                    color: colorScheme.onSurfaceVariant,
                    width: 0.8,
                  ),
                ),
              )
              : null,
      child: Row(
        children: [
          Text(
            label,
            style: TextStyle(
              fontWeight: FontWeight.bold,
              color: colorScheme.onSurfaceVariant,
            ),
          ),
          const Spacer(),
          Checkbox(
            value: value,
            onChanged:
                onChanged != null ? (val) => onChanged!(value: val) : null,
            activeColor: activeColor,
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(4),
            ),
            side: BorderSide(color: colorScheme.outline, width: 1.5),
          ),
        ],
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('label', label));
    properties.add(DiagnosticsProperty<bool>('value', value));
    properties.add(DiagnosticsProperty<bool>('underline', underline));
    properties.add(DoubleProperty('width', width));
    properties.add(DoubleProperty('height', height));
    properties.add(DiagnosticsProperty<EdgeInsetsGeometry?>('margin', margin));
    properties.add(
      ObjectFlagProperty<void Function({required bool? value})?>.has(
        'onChanged',
        onChanged,
      ),
    );
  }
}
