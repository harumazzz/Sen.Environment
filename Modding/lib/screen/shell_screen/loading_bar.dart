import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';

class LoadingBar extends StatefulWidget {
  const LoadingBar({super.key, this.progress});
  final double? progress;

  @override
  State<LoadingBar> createState() => _LoadingBarState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DoubleProperty('progress', progress));
  }
}

class _LoadingBarState extends State<LoadingBar>
    with SingleTickerProviderStateMixin {
  late AnimationController _animationController;

  @override
  void initState() {
    super.initState();
    _animationController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 500),
    );
    if (widget.progress != null) {
      _animationController.value = widget.progress!;
    }
  }

  @override
  void didUpdateWidget(covariant LoadingBar oldWidget) {
    super.didUpdateWidget(oldWidget);
    if (widget.progress != null) {
      _animationController.animateTo(
        widget.progress!.clamp(0.0, 1.0),
        curve: Curves.easeInOut,
        duration: const Duration(milliseconds: 500),
      );
    }
  }

  @override
  void dispose() {
    _animationController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 16.0),
      child: ClipRRect(
        borderRadius: BorderRadius.circular(8.0),
        child: AnimatedBuilder(
          animation: _animationController,
          builder: (context, child) {
            return LinearProgressIndicator(
              value:
                  widget.progress == null ? null : _animationController.value,
              backgroundColor: Colors.grey.shade300,
              valueColor: AlwaysStoppedAnimation<Color>(
                Theme.of(context).colorScheme.primary,
              ),
              minHeight: 6.0,
            );
          },
        ),
      ),
    );
  }
}
