import 'package:flutter/material.dart';
import 'package:loading_animation_widget/loading_animation_widget.dart';

class EmptyBar extends StatelessWidget {
  const EmptyBar({
    super.key,
  });

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: SizedBox(
        height: 46.0,
        width: double.infinity,
        child: FloatingActionButton(
          onPressed: null,
          child: Row(
            crossAxisAlignment: CrossAxisAlignment.center,
            mainAxisSize: MainAxisSize.min,
            spacing: 10.0,
            children: [
              LoadingAnimationWidget.progressiveDots(
                color: Theme.of(context).colorScheme.secondary,
                size: 40,
              ),
            ],
          ),
        ),
      ),
    );
  }
}
