import 'package:flutter/material.dart';

class HorizIcon extends StatefulWidget {
  const HorizIcon({super.key});

  @override
  State<HorizIcon> createState() => _HorizIconState();
}

class _HorizIconState extends State<HorizIcon>
    with SingleTickerProviderStateMixin {
  late AnimationController _controller;
  late Animation<double> _steamAnimation;
  late Animation<double> _opacityAnimation;

  @override
  void initState() {
    super.initState();
    _controller = AnimationController(
      duration: const Duration(milliseconds: 1500),
      vsync: this,
    )..repeat(reverse: false);

    _steamAnimation = Tween<double>(begin: 0, end: -20).animate(
      CurvedAnimation(
        parent: _controller,
        curve: Curves.easeInOut,
      ),
    );
    _opacityAnimation = Tween<double>(begin: 1, end: 0).animate(
      CurvedAnimation(
        parent: _controller,
        curve: Curves.easeIn,
      ),
    );
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      height: 40,
      child: Stack(
        alignment: Alignment.bottomCenter,
        children: [
          const Icon(Icons.coffee_outlined, size: 30),
          AnimatedBuilder(
            animation: _controller,
            builder: (context, child) {
              return Positioned(
                bottom: 25,
                child: Transform.translate(
                  offset: Offset(0, _steamAnimation.value),
                  child: Opacity(
                    opacity: _opacityAnimation.value,
                    child: Icon(
                      Icons.waves_outlined,
                      color: Colors.grey.shade400,
                      size: 15,
                    ),
                  ),
                ),
              );
            },
          ),
        ],
      ),
    );
  }
}
