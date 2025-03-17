import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';

class DropdownButtonField<T> extends StatefulWidget {
  const DropdownButtonField({
    super.key,
    required this.label,
    required this.value,
    required this.items,
    required this.onChanged,
  });

  final String label;
  final T value;
  final List<DropdownMenuItem<T>> items;
  final void Function(T?) onChanged;

  @override
  State<DropdownButtonField<T>> createState() => _DropdownButtonFieldState<T>();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<void Function(T? p1)>.has('onChanged', onChanged),
    );
    properties.add(StringProperty('label', label));
    properties.add(DiagnosticsProperty<T>('value', value));
  }
}

class _DropdownButtonFieldState<T> extends State<DropdownButtonField<T>> {
  late final FocusNode _focusNode;
  late T _selectedValue;
  late TextEditingController _controller;

  @override
  void initState() {
    super.initState();
    _focusNode = FocusNode();
    _selectedValue = widget.value;
    _controller = TextEditingController(text: _selectedValue.toString());
  }

  @override
  void dispose() {
    _focusNode.dispose();
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(
      builder: (context, constraint) {
        return MenuAnchor(
          style: MenuStyle(
            minimumSize: WidgetStatePropertyAll(
              Size(constraint.maxWidth + 8, 0),
            ),
            maximumSize: WidgetStatePropertyAll(
              Size(constraint.maxWidth + 8, double.infinity),
            ),
            elevation: WidgetStateProperty.all(4.0),
          ),
          crossAxisUnconstrained: false,
          alignmentOffset: const Offset(0, 8),
          menuChildren: [
            ...widget.items.map((item) {
              return MenuItemButton(
                onPressed: () {
                  _selectItem(item.value);
                  _focusNode.unfocus();
                },
                child: ListTile(
                  contentPadding: const EdgeInsets.symmetric(horizontal: 16.0),
                  title: item.child,
                ),
              );
            }),
          ],
          builder: (context, controller, child) {
            return TextField(
              minLines: 1,
              maxLines: null,
              focusNode: _focusNode,
              readOnly: true,
              decoration: InputDecoration(
                labelText: widget.label,
                labelStyle: Theme.of(
                  context,
                ).textTheme.bodyLarge?.copyWith(fontWeight: FontWeight.w600),
                contentPadding: const EdgeInsets.symmetric(
                  horizontal: 16.0,
                  vertical: 12.0,
                ),
                floatingLabelBehavior: FloatingLabelBehavior.auto,
                border: OutlineInputBorder(
                  borderSide: BorderSide(
                    color: Theme.of(context).colorScheme.outlineVariant,
                  ),
                ),
                enabledBorder: OutlineInputBorder(
                  borderSide: BorderSide(
                    color: Theme.of(context).colorScheme.outline,
                  ),
                ),
                focusedBorder: OutlineInputBorder(
                  borderSide: BorderSide(
                    color: Theme.of(context).colorScheme.primary,
                    width: 2.0,
                  ),
                ),
                suffixIcon: Icon(
                  Symbols.arrow_drop_down,
                  color: Theme.of(context).colorScheme.onSurfaceVariant,
                ),
              ),
              onTap: () {
                if (controller.isOpen) {
                  controller.close();
                } else {
                  controller.open();
                }
              },
              controller: _controller,
              onTapOutside: (event) {
                _focusNode.unfocus();
              },
            );
          },
        );
      },
    );
  }

  void _selectItem(T? value) {
    if (value != null) {
      setState(() => _selectedValue = value);
      widget.onChanged(value);
      _controller.text = _selectedValue.toString();
    }
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('label', widget.label));
    properties.add(DiagnosticsProperty<T>('value', widget.value));
    properties.add(
      ObjectFlagProperty<void Function(T?)>.has('onChanged', widget.onChanged),
    );
  }
}
