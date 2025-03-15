import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'text_field_padding_fix.dart';
import '../models/map_const.dart';

class NumberTextField extends StatelessWidget {
  const NumberTextField({
    super.key,
    required this.controller,
    required this.range,
    this.onFieldSubmitted,
    this.label = '',
    this.useChangeButton = false,
    this.changeStep = 1,
    this.readOnly = false,
    this.arrowsWidth = 24,
    this.arrowsHeight = kMinInteractiveDimension,
    this.borderWidth = 2,
    this.autovalidateMode = AutovalidateMode.onUnfocus,
    this.validator,
  });

  final String label;
  final NumberEditingController controller;
  final Range<num> range;
  final bool useChangeButton;
  final num changeStep;
  final bool readOnly;
  final double arrowsWidth;
  final double arrowsHeight;
  final double borderWidth;
  final AutovalidateMode autovalidateMode;
  final String? Function(num?)? validator;
  final ValueChanged<num>? onFieldSubmitted;

  num get _numValue =>
      controller.isDouble
          ? double.tryParse(controller.value.text) ?? 0.0
          : int.tryParse(controller.value.text) ?? 0;

  bool _checkRange(num value) {
    return value >= range.begin && value <= range.end;
  }

  RegExp _formatterAllowReg(bool isDouble) {
    final isNegative = range.begin.isNegative;
    if (isNegative && isDouble) {
      return RegExp('[0-9.-]');
    } else if (isDouble) {
      return RegExp('[0-9.]');
    } else if (isNegative) {
      return RegExp('[0-9-]');
    } else {
      return RegExp('[0-9]');
    }
  }

  int _numberOfPrecision(String s) {
    if (s.isEmpty) {
      return 0;
    }
    final indexPrecision = s.indexOf('.');
    if (indexPrecision != -1) {
      return s.length - indexPrecision - 1;
    } else {
      return 0;
    }
  }

  String _addCharAtPosition(
    String s,
    String char,
    int position, {
    bool repeat = false,
  }) {
    if (!repeat) {
      if (s.length < position) {
        return s;
      }
      var before = s.substring(0, position);
      var after = s.substring(position, s.length);
      return before + char + after;
    } else {
      if (position == 0) {
        return s;
      }
      var buffer = StringBuffer();
      for (var i = 0; i < s.length; i++) {
        if (i != 0 && i % position == 0) {
          buffer.write(char);
        }
        buffer.write(String.fromCharCode(s.runes.elementAt(i)));
      }
      return buffer.toString();
    }
  }

  List<TextInputFormatter> _formatter() {
    final isDouble = controller.isDouble;
    return [
      FilteringTextInputFormatter.allow(_formatterAllowReg(isDouble)),
      TextInputFormatter.withFunction((
        TextEditingValue oldValue,
        TextEditingValue newValue,
      ) {
        final newValueText = newValue.text;
        if (newValueText == '-' || newValueText == '.') {
          return newValue;
        } else if (newValueText.isNotEmpty) {
          if (isDouble) {
            final doubleValue = double.tryParse(newValueText);
            if (doubleValue != null) {
              final numOfPrecision = _numberOfPrecision(newValueText);
              final selectionOffset = newValue.selection.baseOffset;
              if ((numOfPrecision == 0 ||
                      newValueText.length - selectionOffset > 3) &&
                  !_checkRange(doubleValue)) {
                final newText = _addCharAtPosition(
                  newValueText,
                  '.',
                  newValueText.length - 1,
                );
                final newDouble = double.tryParse(newText);
                if (newDouble != null && _checkRange(newDouble)) {
                  return newValue.copyWith(
                    text: newText,
                    selection: TextSelection.collapsed(
                      offset: selectionOffset + 1,
                    ),
                  );
                } else {
                  return oldValue;
                }
              }
              if (_checkRange(doubleValue)) {
                if (numOfPrecision < 3) {
                  return newValue;
                }
              }
            }
            return oldValue;
          } else {
            final intValue = int.tryParse(newValueText);
            if (intValue != null && _checkRange(intValue)) {
              return newValue;
            }
            return oldValue;
          }
        } else {
          // Returning new value if text field was empty.
          return newValue;
        }
      }),
    ];
  }

  @override
  Widget build(BuildContext context) {
    final oldValue = controller.value.text;
    return TextFormField(
      controller: controller.value,
      keyboardType: TextInputType.number,
      readOnly: readOnly,
      autovalidateMode: autovalidateMode,
      validator: (value) {
        if (validator != null) {
          return validator!(_numValue);
        }
        return null;
      },
      inputFormatters: _formatter(),
      decoration: InputDecoration(
        errorStyle: const TextStyle(fontSize: 0),
        counterText: '',
        isDense: true,
        labelText: label,
        labelStyle: const TextStyle(fontWeight: FontWeight.bold),
        contentPadding: const EdgeInsets.fromLTRB(12.0, 4.0, 8.0, 12.0),
        suffixIconConstraints: BoxConstraints(
          maxHeight: arrowsHeight,
          maxWidth: arrowsWidth,
        ),
        border: const OutlineInputBorder(),
        suffixIcon: TextFieldPaddingFix(
          child:
              useChangeButton
                  ? NumberIncrementDecrement(
                    controller: controller,
                    range: range,
                    changeStep: changeStep,
                    onChanged: onFieldSubmitted,
                  )
                  : null,
        ),
      ),
      onTapOutside: (value) {
        controller.value.text = oldValue;
      },
      onFieldSubmitted:
          onFieldSubmitted != null
              ? (value) {
                if (value == '') {
                  controller.value.text = _numValue.toString();
                }
                onFieldSubmitted!(_numValue);
              }
              : null,
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('label', label));
    properties.add(
      DiagnosticsProperty<NumberEditingController>('controller', controller),
    );
    properties.add(DiagnosticsProperty<Range<num>>('range', range));
    properties.add(
      DiagnosticsProperty<bool>('useChangeButton', useChangeButton),
    );
    properties.add(DiagnosticsProperty<num>('changeStep', changeStep));
    properties.add(DiagnosticsProperty<bool>('readOnly', readOnly));
    properties.add(DoubleProperty('arrowsWidth', arrowsWidth));
    properties.add(DoubleProperty('arrowsHeight', arrowsHeight));
    properties.add(DoubleProperty('borderWidth', borderWidth));
    properties.add(
      EnumProperty<AutovalidateMode>('autovalidateMode', autovalidateMode),
    );
    properties.add(
      ObjectFlagProperty<String? Function(num? p1)?>.has(
        'validator',
        validator,
      ),
    );
    properties.add(
      ObjectFlagProperty<ValueChanged<num>?>.has(
        'onFieldSubmitted',
        onFieldSubmitted,
      ),
    );
  }
}

class NumberIncrementDecrement extends StatelessWidget {
  const NumberIncrementDecrement({
    super.key,
    required this.controller,
    required this.changeStep,
    required this.range,
    this.onChanged,
  });

  final NumberEditingController controller;
  final Range<num> range;
  final num changeStep;
  final ValueChanged<num>? onChanged;

  num get _numValue =>
      controller.isDouble
          ? double.tryParse(controller.value.text) ?? 0.0
          : int.tryParse(controller.value.text) ?? 0;

  bool _isMin({num? value}) {
    return range.begin < (value ?? _numValue);
  }

  bool _isMax({num? value}) {
    return range.end > (value ?? _numValue);
  }

  void _changeUpValue() {
    final newValue = _numValue + changeStep;
    if (onChanged != null) {
      onChanged!(newValue);
    }
    controller.value.text =
        controller.isDouble
            ? newValue.toStringAsFixed(2)
            : newValue.toStringAsFixed(0);
  }

  void _changeDownValue() {
    final newValue = _numValue - changeStep;
    if (onChanged != null) {
      onChanged!(newValue);
    }
    controller.value.text =
        controller.isDouble
            ? newValue.toStringAsFixed(2)
            : newValue.toStringAsFixed(0);
  }

  @override
  Widget build(BuildContext context) {
    return ValueListenableBuilder(
      valueListenable: controller.value,
      builder: (context, value, child) {
        return Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            Expanded(
              child: InkWell(
                onTap: _isMax() ? _changeUpValue : null,
                child: Opacity(
                  opacity: _isMax() ? 1 : .5,
                  child: const Icon(Icons.arrow_drop_up),
                ),
              ),
            ),
            Expanded(
              child: InkWell(
                onTap: _isMin() ? _changeDownValue : null,
                child: Opacity(
                  opacity: _isMin() ? 1 : .5,
                  child: const Icon(Icons.arrow_drop_down),
                ),
              ),
            ),
          ],
        );
      },
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<NumberEditingController>('controller', controller),
    );
    properties.add(DiagnosticsProperty<Range<num>>('range', range));
    properties.add(DiagnosticsProperty<num>('changeStep', changeStep));
    properties.add(
      ObjectFlagProperty<ValueChanged<num>?>.has('onChanged', onChanged),
    );
  }
}

class NumberEditingController extends ValueNotifier<TextEditingController> {
  NumberEditingController({required num value, required this.isDouble})
    : super(
        TextEditingController(
          text: isDouble ? value.toStringAsFixed(2) : value.toStringAsFixed(0),
        ),
      );

  final bool isDouble;
}
