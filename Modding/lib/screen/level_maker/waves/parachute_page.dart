import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/model/wave.dart';
import 'package:sen/screen/level_maker/zombie_suggestion.dart';

class ParachutePage extends StatefulWidget {
  const ParachutePage({
    super.key,
    required this.index,
    required this.wave,
    required this.zombies,
  });

  final int index;

  final ParachuteRain wave;

  final List<String> zombies;

  @override
  State<ParachutePage> createState() => _ParachutePageState();
}

class _ParachutePageState extends State<ParachutePage> {
  late GlobalKey<FormState> _formKey;
  late TextEditingController _columnStartController;
  late TextEditingController _columnEndController;
  late TextEditingController _countController;
  late TextEditingController _fallTimeController;
  late TextEditingController _groupSizeController;
  late TextEditingController _messageController;
  late TextEditingController _timeBetweenGroupController;
  late TextEditingController _typenameController;

  @override
  void initState() {
    _formKey = GlobalKey<FormState>();
    _columnStartController = TextEditingController(
      text: widget.wave.columnStart.toString(),
    );
    _columnEndController = TextEditingController(
      text: widget.wave.columnEnd.toString(),
    );
    _countController = TextEditingController(
      text: widget.wave.count.toString(),
    );
    _fallTimeController = TextEditingController(
      text: widget.wave.fallTime.toString(),
    );
    _groupSizeController = TextEditingController(
      text: widget.wave.groupSize.toString(),
    );
    _messageController = TextEditingController(
      text: widget.wave.message,
    );
    _timeBetweenGroupController = TextEditingController(
      text: widget.wave.timeBetweenGroups.toString(),
    );
    _typenameController = TextEditingController(
      text: widget.wave.typename,
    );
    super.initState();
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  @override
  void dispose() {
    _columnStartController.dispose();
    _columnEndController.dispose();
    _countController.dispose();
    _fallTimeController.dispose();
    _groupSizeController.dispose();
    _messageController.dispose();
    _timeBetweenGroupController.dispose();
    _typenameController.dispose();
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

  Widget _buildCard({
    required String title,
    required List<Widget> children,
  }) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.symmetric(
          vertical: 12.0,
          horizontal: 16.0,
        ),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          mainAxisAlignment: MainAxisAlignment.start,
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

  void _onSubmit() {
    if (_formKey.currentState!.validate()) {
      widget.wave.replaceWith(
        columnStart: int.parse(_columnStartController.text),
        columnEnd: int.parse(_columnEndController.text),
        count: int.parse(_countController.text),
        fallTime: double.parse(_fallTimeController.text),
        groupSize: int.parse(_groupSizeController.text),
        timeBetweenGroups: double.parse(_timeBetweenGroupController.text),
        message: _messageController.text,
        typename: _typenameController.text,
      );
      Navigator.of(context).pop();
    }
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Scaffold(
      appBar: AppBar(
        title: Text('${los.wave} ${widget.index}: ${los.parachute_rain}'),
      ),
      body: Form(
        key: _formKey,
        child: Padding(
          padding: const EdgeInsets.all(8.0),
          child: SingleChildScrollView(
            child: Column(
              children: [
                _buildCard(
                  title: los.entry,
                  children: [
                    _buildFormField(
                      labelText: '${los.column_start}:',
                      controller: _columnStartController,
                      keyboardType: TextInputType.number,
                      inputFormatters: [FilteringTextInputFormatter.digitsOnly],
                    ),
                    _buildFormField(
                      labelText: '${los.column_end}:',
                      controller: _columnEndController,
                      keyboardType: TextInputType.number,
                      inputFormatters: [FilteringTextInputFormatter.digitsOnly],
                    ),
                    _buildFormField(
                      labelText: '${los.count}:',
                      controller: _countController,
                      keyboardType: TextInputType.number,
                      inputFormatters: [FilteringTextInputFormatter.digitsOnly],
                    ),
                    _buildFormField(
                      labelText: '${los.delay_between_groups}:',
                      controller: _timeBetweenGroupController,
                      keyboardType:
                          const TextInputType.numberWithOptions(decimal: true),
                      inputFormatters: [FilteringTextInputFormatter.digitsOnly],
                    ),
                    _buildFormField(
                      labelText: '${los.fall_time}:',
                      controller: _fallTimeController,
                      keyboardType:
                          const TextInputType.numberWithOptions(decimal: true),
                      inputFormatters: [
                        FilteringTextInputFormatter.allow(RegExp(r'^\d*\.?\d*'))
                      ],
                    ),
                    _buildFormField(
                      labelText: '${los.group_size}:',
                      controller: _groupSizeController,
                      keyboardType:
                          const TextInputType.numberWithOptions(decimal: true),
                      inputFormatters: [
                        FilteringTextInputFormatter.allow(RegExp(r'^\d*\.?\d*'))
                      ],
                    ),
                    TextField(
                      controller: _messageController,
                      decoration: InputDecoration(
                        labelText: los.start_message,
                      ),
                    ),
                  ],
                ),
                _buildCard(
                  title: los.zombie,
                  children: [
                    ZombieSelection(
                      controller: _typenameController,
                      zombies: widget.zombies,
                    ),
                  ],
                ),
                const SizedBox(height: 20.0),
                ElevatedButton(
                  onPressed: _onSubmit,
                  child: SizedBox(
                    width: double.infinity,
                    child: Padding(
                      padding: const EdgeInsets.symmetric(vertical: 16.0),
                      child: Center(child: Text(los.save)),
                    ),
                  ),
                ),
                const SizedBox(height: 20.0),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
