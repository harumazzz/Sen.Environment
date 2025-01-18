import 'package:flutter/material.dart';

class CheckBoxField extends StatelessWidget {
  const CheckBoxField(
      {super.key,
      required this.label,
      required this.value,
      required this.onChanged,
      this.width,
      this.height,
      this.margin,
      this.underline = true});

  final String label;

  final bool value;

  final bool underline;

  final double? width;

  final double? height;

  final EdgeInsetsGeometry? margin;

  final void Function(bool?)? onChanged;

  @override
  Widget build(BuildContext context) {
    return Container(
      width: width ?? double.infinity,
      height: height,
      margin: margin ?? const EdgeInsets.fromLTRB(12, 16, 20, 16),
      decoration: underline
          ? BoxDecoration(
              border: Border(
                  bottom: BorderSide(
                      color:
                          Theme.of(context).colorScheme.onSecondaryFixedVariant,
                      width: 0.8)))
          : null,
      child: Row(
        children: [
          Text(
            label,
            style: TextStyle(
                fontWeight: FontWeight.bold,
                color: Theme.of(context).colorScheme.onSecondaryFixedVariant),
          ),
          const Spacer(),
          Checkbox(
              value: value,
              activeColor: Theme.of(context).colorScheme.primaryFixedDim,
              onChanged: onChanged)
        ],
      ),
    );
  }
}
