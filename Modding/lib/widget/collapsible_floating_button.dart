import 'package:equatable/equatable.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';

class CollapsibleFloatingActionButton extends StatefulWidget {
  const CollapsibleFloatingActionButton({super.key, required this.items});
  final List<CollapsibleFloatingActionButtonItem> items;

  @override
  State<CollapsibleFloatingActionButton> createState() =>
      _CollapsibleFloatingActionButtonState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      IterableProperty<CollapsibleFloatingActionButtonItem>('items', items),
    );
  }
}

class CollapsibleFloatingActionButtonItem extends Equatable {
  const CollapsibleFloatingActionButtonItem({
    required this.icon,
    required this.tooltip,
    required this.onPressed,
  });
  final IconData icon;
  final String tooltip;
  final VoidCallback onPressed;

  @override
  List<Object> get props => [icon, tooltip, onPressed];
}

class _CollapsibleFloatingActionButtonState
    extends State<CollapsibleFloatingActionButton>
    with SingleTickerProviderStateMixin {
  late AnimationController _animationController;
  late Animation<double> _rotateAnimation;
  late Animation<double> _fadeAnimation;
  late Animation<Offset> _slideAnimation;
  bool _isExpanded = false;

  @override
  void initState() {
    super.initState();
    _animationController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 300),
    );
    _rotateAnimation = Tween<double>(begin: 0.0, end: 45.0).animate(
      CurvedAnimation(parent: _animationController, curve: Curves.easeOut),
    );
    _fadeAnimation = CurvedAnimation(
      parent: _animationController,
      curve: Curves.easeInOut,
    );
    _slideAnimation = Tween<Offset>(
      begin: const Offset(0, 0.2),
      end: Offset.zero,
    ).animate(
      CurvedAnimation(parent: _animationController, curve: Curves.easeOut),
    );
  }

  @override
  void dispose() {
    _animationController.dispose();
    super.dispose();
  }

  void _toggle() {
    setState(() {
      _isExpanded = !_isExpanded;
      if (_isExpanded) {
        _animationController.forward();
      } else {
        _animationController.reverse();
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      mainAxisSize: MainAxisSize.min,
      children: [
        if (_isExpanded) ..._buildExpandedOptions(),
        AnimatedBuilder(
          animation: _animationController,
          builder: (context, child) {
            return Transform.rotate(
              angle: _rotateAnimation.value * (3.14159265359 / 180),
              child: FloatingActionButton(
                onPressed: _toggle,
                child: Icon(_isExpanded ? Symbols.close : Symbols.add),
              ),
            );
          },
        ),
      ],
    );
  }

  List<Widget> _buildExpandedOptions() {
    final options = <Widget>[];
    for (var i = 0; i < widget.items.length; i++) {
      options.add(
        SlideTransition(
          position: _slideAnimation,
          child: FadeTransition(
            opacity: _fadeAnimation,
            child: Padding(
              padding: const EdgeInsets.only(bottom: 24.0),
              child: FloatingActionButton(
                tooltip: widget.items[i].tooltip,
                mini: true,
                onPressed: () {
                  widget.items[i].onPressed();
                  _toggle();
                },
                child: Icon(widget.items[i].icon),
              ),
            ),
          ),
        ),
      );
    }
    return options;
  }
}
