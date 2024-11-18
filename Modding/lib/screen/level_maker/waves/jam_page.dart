import 'package:flutter/material.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/model/wave.dart';

class JamPage extends StatefulWidget {
  const JamPage({
    super.key,
    required this.index,
    required this.wave,
    required this.notificationEvent,
  });

  final int index;
  final List<String> notificationEvent;
  final JamWave wave;

  @override
  State<JamPage> createState() => _JamPageState();
}

class _JamPageState extends State<JamPage> {
  late String _notificationEvent;

  @override
  void initState() {
    _notificationEvent = widget.wave.notificationEvent;
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
      title: Text('${los.wave} ${widget.index}: ${los.jam_wave}'),
    );
  }

  Widget _buildBody(AppLocalizations los) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: SingleChildScrollView(
        child: Column(
          children: [
            _buildTidalCard(context, los),
            const SizedBox(height: 20.0),
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
          children: [
            _buildCardHeader(context, los),
            const SizedBox(height: 16.0),
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
      style: Theme.of(context)
          .textTheme
          .titleLarge
          ?.copyWith(fontWeight: FontWeight.bold),
    );
  }

  Widget _buildDropdownRow(AppLocalizations los) {
    return Row(
      children: [
        Text('${los.notification_event}:'),
        const SizedBox(width: 15.0),
        Expanded(
          child: DropdownButton<String>(
            items: _buildDropdownItems(),
            value: _notificationEvent,
            onChanged: (String? value) {
              if (value != null) {
                setState(() {
                  _notificationEvent = value;
                });
              }
            },
          ),
        ),
      ],
    );
  }

  List<DropdownMenuItem<String>> _buildDropdownItems() {
    return widget.notificationEvent
        .map<DropdownMenuItem<String>>(
          (e) => DropdownMenuItem(
            value: e,
            child: Text(e),
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
    widget.wave.replaceWith(notificationEvent: _notificationEvent);
    Navigator.of(context).pop();
  }
}
