import 'package:flutter/material.dart';
import 'package:sen/model/wave.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class RegularPage extends StatefulWidget {
  const RegularPage({
    super.key,
    required this.wave,
    required this.index,
  });

  final RegularWave wave;

  final int index;

  @override
  State<RegularPage> createState() => _RegularPageState();
}

class _RegularPageState extends State<RegularPage> {
  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    widget.wave.zombies.add(Spawn(row: 1, typename: 'tutorial'));
    return Scaffold(
      appBar: AppBar(
        title: Text('${los.wave} ${widget.index}'),
      ),
      body: const Column(
        children: [],
      ),
    );
  }
}
