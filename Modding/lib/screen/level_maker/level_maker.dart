import 'package:flutter/material.dart';
import '../../extension/context.dart';
import '../../extension/platform.dart';

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
