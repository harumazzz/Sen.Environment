import 'package:flutter/material.dart';
import '../../extension/context.dart';
import '../../service/ui_helper.dart';

class LevelMaker extends StatelessWidget {
  const LevelMaker({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: UIHelper.appBarOr(AppBar(title: Text(context.los.level_maker))),
    );
  }
}
