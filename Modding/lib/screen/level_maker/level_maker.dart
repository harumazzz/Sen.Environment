import 'package:flutter/material.dart';
import 'package:sen/extension/context.dart';
import 'package:sen/extension/platform.dart';

class LevelMaker extends StatelessWidget {
  const LevelMaker({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        forceMaterialTransparency: CurrentPlatform.isDesktop,
        title: Text(context.los.level_maker),
      ),
    );
  }
}
