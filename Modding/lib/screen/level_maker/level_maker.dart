import 'package:flutter/material.dart';
import '../../extension/context.dart';
import '../../service/ui_helper.dart';

class LevelMaker extends StatelessWidget {
  const LevelMaker({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: UIHelper.ofMobile(
        builder:
            () => AppBar(
              title: Text(
                context.los.level_maker,
                style: const TextStyle(fontWeight: FontWeight.bold),
              ),
            ),
      ),
    );
  }
}
