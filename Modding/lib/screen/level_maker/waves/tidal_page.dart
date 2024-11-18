import 'package:flutter/material.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/model/wave.dart';

class TidalPage extends StatefulWidget {
  const TidalPage({
    super.key,
    required this.index,
    required this.wave,
  });

  final int index;

  final TidalChange wave;

  @override
  State<TidalPage> createState() => _TidalPageState();
}

class _TidalPageState extends State<TidalPage> {
  late int _changeAmount;

  @override
  void initState() {
    _changeAmount = widget.wave.changeAmount;
    super.initState();
  }

  @override
  void dispose() {
    super.dispose();
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  Widget _buildSaveButton(AppLocalizations los) {
    return ElevatedButton(
      onPressed: _onSubmit,
      child: SizedBox(
        width: double.infinity,
        child: Padding(
          padding: const EdgeInsets.symmetric(vertical: 16.0),
          child: Center(child: Text(los.save)),
        ),
      ),
    );
  }

  void _onSubmit() {
    widget.wave.replaceWith(changeAmount: _changeAmount);
    Navigator.of(context).pop();
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Scaffold(
      appBar: AppBar(
        title: Text('${los.wave} ${widget.index}: ${los.tidal_change}'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(8.0),
        child: SingleChildScrollView(
          child: Column(
            children: [
              Card(
                child: Padding(
                  padding: const EdgeInsets.symmetric(
                    vertical: 12.0,
                    horizontal: 16.0,
                  ),
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.start,
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        los.entry,
                        style: Theme.of(context)
                            .textTheme
                            .titleLarge
                            ?.copyWith(fontWeight: FontWeight.bold),
                      ),
                      const SizedBox(height: 16.0),
                      Row(
                        children: [
                          Text('${los.tide_coordinate}:'),
                          const SizedBox(width: 15.0),
                          Expanded(
                            child: DropdownButton<int>(
                              items: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
                                  .map<DropdownMenuItem<int>>(
                                    (e) => DropdownMenuItem(
                                      child: Text(e.toString()),
                                      value: e,
                                    ),
                                  )
                                  .toList(),
                              value: _changeAmount,
                              onChanged: (int? value) {
                                if (value == null) return;
                                setState(() {
                                  _changeAmount = value;
                                });
                              },
                            ),
                          ),
                        ],
                      ),
                    ],
                  ),
                ),
              ),
              const SizedBox(height: 20.0),
              _buildSaveButton(los),
            ],
          ),
        ),
      ),
    );
  }
}
