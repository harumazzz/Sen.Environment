import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:sen/model/wave.dart';
import 'package:sen/screen/level_maker/zombie_suggestion.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class LowTidePage extends StatefulWidget {
  const LowTidePage({
    super.key,
    required this.wave,
    required this.index,
    required this.zombies,
  });

  final LowTide wave;

  final int index;

  final List<String> zombies;

  @override
  State<LowTidePage> createState() => _LowTidePageState();
}

class _LowTidePageState extends State<LowTidePage> {
  late TextEditingController _xStart;

  late TextEditingController _xEnd;

  late TextEditingController _groupSize;

  late TextEditingController _delay;

  late TextEditingController _message;

  late TextEditingController _zombie;

  late TextEditingController _count;

  late GlobalKey<FormState> _formKey;

  @override
  void initState() {
    _xStart = TextEditingController(text: widget.wave.xStart.toString());
    _xEnd = TextEditingController(text: widget.wave.xEnd.toString());
    _groupSize = TextEditingController(text: widget.wave.groupSize.toString());
    _delay = TextEditingController(text: widget.wave.delay.toString());
    _message = TextEditingController(text: widget.wave.message.toString());
    _zombie = TextEditingController(text: widget.wave.zombieType.toString());
    _count = TextEditingController(text: widget.wave.count.toString());
    _formKey = GlobalKey<FormState>();
    super.initState();
  }

  @override
  void dispose() {
    _xStart.dispose();
    _xEnd.dispose();
    _groupSize.dispose();
    _delay.dispose();
    _message.dispose();
    _zombie.dispose();
    _count.dispose();
    super.dispose();
  }

  Widget _buildFormField({
    required String labelText,
    required TextEditingController controller,
    required TextInputType keyboardType,
    required List<TextInputFormatter> inputFormatters,
  }) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 10.0),
      child: TextFormField(
        controller: controller,
        decoration: InputDecoration(labelText: labelText),
        keyboardType: keyboardType,
        inputFormatters: inputFormatters,
        validator: (value) {
          if (value == null) {
            return AppLocalizations.of(context)!.value_cannot_be_null;
          }
          if (value.isEmpty) {
            return AppLocalizations.of(context)!.value_cannot_be_empty;
          }
          return null;
        },
      ),
    );
  }

  void _onSave() {
    if (_formKey.currentState!.validate()) {
      widget.wave.replaceWith(
        xStart: int.parse(_xStart.text),
        xEnd: int.parse(_xEnd.text),
        count: int.parse(_count.text),
        delay: double.parse(_delay.text),
        groupSize: int.parse(_groupSize.text),
        message: _message.text,
        zombieType: _zombie.text,
      );
      Navigator.of(context).pop();
    }
  }

  Widget _buildCard({
    required String title,
    required List<Widget> children,
  }) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(12.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              title,
              style: Theme.of(context)
                  .textTheme
                  .titleLarge
                  ?.copyWith(fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 20.0),
            ...children,
          ],
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;

    return Scaffold(
      appBar: AppBar(
        title: Text('${los.wave} ${widget.index}: ${los.low_tide}'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(12.0),
        child: Form(
          key: _formKey,
          child: SingleChildScrollView(
            child: Column(
              children: [
                _buildCard(
                  title: los.entry,
                  children: [
                    _buildFormField(
                      labelText: los.column_start,
                      controller: _xStart,
                      keyboardType: TextInputType.number,
                      inputFormatters: [FilteringTextInputFormatter.digitsOnly],
                    ),
                    _buildFormField(
                      labelText: los.column_end,
                      controller: _xEnd,
                      keyboardType: TextInputType.number,
                      inputFormatters: [FilteringTextInputFormatter.digitsOnly],
                    ),
                    _buildFormField(
                      labelText: los.group_size,
                      controller: _groupSize,
                      keyboardType: TextInputType.number,
                      inputFormatters: [FilteringTextInputFormatter.digitsOnly],
                    ),
                    _buildFormField(
                      labelText: los.delay_between_groups,
                      controller: _delay,
                      keyboardType:
                          const TextInputType.numberWithOptions(decimal: true),
                      inputFormatters: [
                        FilteringTextInputFormatter.allow(RegExp(r'^\d*\.?\d*'))
                      ],
                    ),
                    TextField(
                      controller: _message,
                      decoration: InputDecoration(labelText: los.start_message),
                    ),
                  ],
                ),
                const SizedBox(height: 10.0),
                const Divider(thickness: 1.0),
                const SizedBox(height: 10.0),
                _buildCard(
                  title: los.zombie,
                  children: [
                    ZombieSelection(
                      controller: _zombie,
                      zombies: widget.zombies,
                    ),
                    const SizedBox(height: 10.0),
                    _buildFormField(
                      labelText: los.count,
                      controller: _count,
                      keyboardType: TextInputType.number,
                      inputFormatters: [FilteringTextInputFormatter.digitsOnly],
                    ),
                  ],
                ),
                const SizedBox(height: 20.0),
                ElevatedButton(
                  onPressed: _onSave,
                  child: SizedBox(
                    width: double.infinity,
                    child: Center(
                      child: Padding(
                        padding: const EdgeInsets.symmetric(vertical: 20.0),
                        child: Text(los.save),
                      ),
                    ),
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
