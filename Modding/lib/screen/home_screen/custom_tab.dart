import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';

import '../../extension/context.dart';

class CustomTab extends StatefulWidget {
  const CustomTab({
    required this.icon,
    required this.title,
    required this.isSelected,
    this.onClose,
    this.onSecondaryTapDown,
    super.key,
  });

  final Widget icon;
  final String title;
  final VoidCallback? onClose;
  final bool isSelected;
  final void Function(TapDownDetails)? onSecondaryTapDown;

  @override
  State<CustomTab> createState() => _CustomTabState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Widget>('icon', icon));
    properties.add(StringProperty('title', title));
    properties.add(ObjectFlagProperty<VoidCallback?>.has('onClose', onClose));
    properties.add(DiagnosticsProperty<bool>('isSelected', isSelected));
    properties.add(
      ObjectFlagProperty<void Function(TapDownDetails p1)?>.has(
        'onSecondaryTapDown',
        onSecondaryTapDown,
      ),
    );
  }
}

class _CustomTabState extends State<CustomTab> {
  late bool _isHovered;

  @override
  void initState() {
    _isHovered = false;
    super.initState();
  }

  Color? _colorDark() {
    return _isHovered ? Colors.grey[800] : Colors.grey[900];
  }

  Color? _colorLight() {
    return _isHovered ? Colors.grey[300] : Colors.grey[200];
  }

  @override
  Widget build(BuildContext context) {
    return MouseRegion(
      onEnter: (_) => setState(() => _isHovered = true),
      onExit: (_) => setState(() => _isHovered = false),
      child: GestureDetector(
        onSecondaryTapDown: widget.onSecondaryTapDown,
        child: AnimatedContainer(
          duration: const Duration(milliseconds: 300),
          width: 215,
          padding: const EdgeInsets.symmetric(horizontal: 8.0, vertical: 3.0),
          decoration: BoxDecoration(
            color: context.isDarkMode ? _colorDark() : _colorLight(),
            borderRadius: const BorderRadius.only(
              topLeft: Radius.circular(16.0),
              topRight: Radius.circular(16.0),
            ),
          ),
          margin: const EdgeInsets.only(left: 4.0, right: 4.0, top: 8.0),
          child: Row(
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            children: [
              Row(
                spacing: 8.0,
                children: [
                  widget.icon,
                  Text(
                    widget.title,
                    style: Theme.of(context).textTheme.labelLarge?.copyWith(
                      fontWeight:
                          widget.isSelected
                              ? FontWeight.bold
                              : FontWeight.normal,
                      color:
                          widget.isSelected
                              ? Theme.of(context).colorScheme.primary
                              : null,
                    ),
                  ),
                ],
              ),
              IconButton(
                tooltip: context.los.close,
                onPressed: widget.onClose,
                icon: const Icon(Symbols.close),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
