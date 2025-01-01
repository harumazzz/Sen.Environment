import 'dart:io';

import 'package:desktop_drop/desktop_drop.dart';
import 'package:flutter/material.dart';

class DropHandler extends StatelessWidget {
  final Widget child;
  final TextEditingController? inputController;

  const DropHandler({
    super.key,
    required this.child,
    required this.inputController,
  });

  @override
  Widget build(BuildContext context) {
    if (Platform.isAndroid || Platform.isIOS) return child;
    return DropTarget(
      onDragDone: (details) {
        if (details.files.isNotEmpty) {
          if (details.files.isNotEmpty) {
            var file = details.files.first;
            inputController?.text = file.path;
          }
        }
      },
      child: child,
    );
  }
}
