import 'package:flutter/material.dart';
import 'package:sen/i18n/app_localizations.dart';
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
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Scaffold(
      appBar: _buildAppBar(los),
      body: _buildBody(los),
    );
  }

  AppBar _buildAppBar(AppLocalizations los) {
    return AppBar(
      title: Text('${los.wave} ${widget.index}: ${los.tidal_change}'),
    );
  }

  Widget _buildBody(AppLocalizations los) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: SingleChildScrollView(
        child: Column(
          spacing: 20.0,
          children: [
            _buildTidalCard(context, los),
            _buildSaveButton(los),
          ],
        ),
      ),
    );
  }

  Widget _buildTidalCard(
    BuildContext context,
    AppLocalizations los,
  ) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.symmetric(
          vertical: 12.0,
          horizontal: 16.0,
        ),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          spacing: 16.0,
          children: [
            _buildCardHeader(context, los),
            _buildDropdownRow(los),
          ],
        ),
      ),
    );
  }

  Widget _buildCardHeader(
    BuildContext context,
    AppLocalizations los,
  ) {
    return Text(
      los.entry,
      style: Theme.of(context).textTheme.titleLarge?.copyWith(fontWeight: FontWeight.bold),
    );
  }

  Widget _buildDropdownRow(AppLocalizations los) {
    return Row(
      spacing: 15.0,
      children: [
        Text('${los.tide_coordinate}:'),
        Expanded(
          child: DropdownButton<int>(
            items: _buildDropdownItems(),
            value: _changeAmount,
            onChanged: (int? value) {
              if (value != null) {
                setState(() {
                  _changeAmount = value;
                });
              }
            },
          ),
        ),
      ],
    );
  }

  List<DropdownMenuItem<int>> _buildDropdownItems() {
    return [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
        .map<DropdownMenuItem<int>>(
          (e) => DropdownMenuItem(
            value: e,
            child: Text(e.toString()),
          ),
        )
        .toList();
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
}
