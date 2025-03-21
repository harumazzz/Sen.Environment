import 'package:flutter/material.dart';

class LoadingBar extends StatefulWidget {
  const LoadingBar({super.key});

  @override
  State<LoadingBar> createState() => _LoadingBarState();
}

class _LoadingBarState extends State<LoadingBar>
    with SingleTickerProviderStateMixin {
  late AnimationController _animationController;
  late Animation<Color?> _colorAnimation;

  @override
  void initState() {
    super.initState();
    _animationController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 1200),
    )..repeat(reverse: true);

    _colorAnimation = ColorTween(
      begin: Colors.transparent,
    ).animate(_animationController);
    WidgetsBinding.instance.addPostFrameCallback((_) {
      _colorAnimation = ColorTween(
        begin: Theme.of(context).colorScheme.primary.withValues(alpha: 0.12),
        end: Theme.of(context).colorScheme.primary,
      ).animate(_animationController);
    });
  }

  @override
  void dispose() {
    _animationController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: ClipRRect(
        borderRadius: BorderRadius.circular(12.0),
        child: AnimatedBuilder(
          animation: _animationController,
          builder: (context, child) {
            return LinearProgressIndicator(
              backgroundColor: Colors.grey.shade300,
              valueColor: AlwaysStoppedAnimation<Color>(_colorAnimation.value!),
              minHeight: 6.0,
            );
          },
        ),
      ),
    );
  }
}
