import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:sen/model/wave.dart';
import 'package:sen/screen/level_maker/zombie_suggestion.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class StormPage extends StatefulWidget {
  const StormPage({
    super.key,
    required this.wave,
    required this.index,
    required this.zombies,
  });

  final StormEvent wave;
  final List<String> zombies;
  final int index;

  @override
  State<StormPage> createState() => _StormPageState();
}

class _StormPageState extends State<StormPage> {
  late TextEditingController _xStart;
  late TextEditingController _xEnd;
  late TextEditingController _groupSize;
  late TextEditingController _delay;
  late TextEditingController _plantfood;
  late GlobalKey<FormState> _formKey;
  late String _eventName;
  late List<String> _zombies;
  late TextEditingController _zombie;

  @override
  void initState() {
    _xStart = TextEditingController(text: widget.wave.columnStart.toString());
    _xEnd = TextEditingController(text: widget.wave.columnEnd.toString());
    _groupSize = TextEditingController(text: widget.wave.groupSize.toString());
    _delay =
        TextEditingController(text: widget.wave.timeBetweenGroups.toString());
    _plantfood =
        TextEditingController(text: widget.wave.additionalPlantfood.toString());
    _zombie = TextEditingController();
    _formKey = GlobalKey<FormState>();
    _eventName = widget.wave.eventName;
    _zombies = [...widget.wave.zombies];
    super.initState();
  }

  @override
  void dispose() {
    _xStart.dispose();
    _xEnd.dispose();
    _groupSize.dispose();
    _delay.dispose();
    _plantfood.dispose();
    _zombie.dispose();
    super.dispose();
  }

  Widget _customTextField({
    required TextEditingController controller,
    required String labelText,
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
      ),
    );
  }

  Widget _stormEvent() {
    setEventChange(String? newEvent) {
      if (newEvent == null) return;
      setState(() {
        _eventName = newEvent;
      });
    }

    final los = AppLocalizations.of(context)!;
    return Row(
      mainAxisSize: MainAxisSize.min,
      children: [
        Expanded(
          child: RadioListTile<String>(
            value: 'sandstorm',
            groupValue: _eventName,
            onChanged: setEventChange,
            title: Text(los.sandstorm),
          ),
        ),
        Expanded(
          child: RadioListTile<String>(
            value: 'snowstorm',
            groupValue: _eventName,
            onChanged: setEventChange,
            title: Text(los.snowstorm),
          ),
        ),
      ],
    );
  }

  Future<void> _onLevelLawnSpawn(
    int row,
    int col,
    Map<int, MemoryImage> cellItems,
    int index,
  ) async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.pick_a_zombie),
        content: ZombiePicker(
          zombies: widget.zombies,
          controller: _zombie,
          onChange: (String value, MemoryImage? image) {
            setState(() {
              _zombie.text = value;
              cellItems[index] = image!;
            });
            Navigator.of(context).pop();
          },
        ),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop();
            },
            child: Text(los.okay),
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Scaffold(
      appBar: AppBar(
        title: Text('${los.wave} ${widget.index}: ${los.storm_event}'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(12.0),
        child: Form(
          key: _formKey,
          child: SingleChildScrollView(
            child: Column(
              children: [
                Column(
                  children: [
                    Card(
                      child: Padding(
                        padding: const EdgeInsets.all(12.0),
                        child: Column(
                          children: [
                            Text(
                              los.entry,
                              style: Theme.of(context)
                                  .textTheme
                                  .titleLarge
                                  ?.copyWith(
                                    fontWeight: FontWeight.bold,
                                  ),
                            ),
                            SizedBox(height: 20.0),
                            TextField(
                              controller: _xStart,
                              decoration: InputDecoration(
                                label: Text(los.column_start),
                              ),
                              enabled: false,
                            ),
                            SizedBox(height: 10.0),
                            TextField(
                              controller: _xEnd,
                              decoration: InputDecoration(
                                label: Text(los.column_end),
                              ),
                              enabled: false,
                            ),
                            _customTextField(
                              controller: _groupSize,
                              labelText: los.group_size,
                              keyboardType: TextInputType.number,
                              inputFormatters: [
                                FilteringTextInputFormatter.digitsOnly,
                              ],
                            ),
                            _customTextField(
                              controller: _delay,
                              labelText: los.delay_between_groups,
                              keyboardType: TextInputType.numberWithOptions(
                                decimal: true,
                              ),
                              inputFormatters: [
                                FilteringTextInputFormatter.allow(
                                  RegExp(r'^\d*\.?\d*'),
                                ),
                              ],
                            ),
                            _customTextField(
                              controller: _plantfood,
                              labelText: los.additional_plantfood,
                              keyboardType: TextInputType.number,
                              inputFormatters: [
                                FilteringTextInputFormatter.digitsOnly,
                              ],
                            ),
                            SizedBox(height: 10.0),
                            _stormEvent(),
                            SizedBox(height: 10.0),
                            Text(
                              los.zombie,
                              style: Theme.of(context)
                                  .textTheme
                                  .titleLarge
                                  ?.copyWith(
                                    fontWeight: FontWeight.bold,
                                  ),
                            ),
                            SizedBox(height: 20.0),
                            LevelLawn(onTap: _onLevelLawnSpawn),
                          ],
                        ),
                      ),
                    ),
                    SizedBox(height: 20.0),
                    ElevatedButton(
                      onPressed: () {
                        if (_formKey.currentState!.validate()) {
                          widget.wave.replaceWith(
                            additionalPlantfood: int.parse(_plantfood.text),
                            columnStart: int.parse(_xStart.text),
                            columnEnd: int.parse(_xEnd.text),
                            eventName: _eventName,
                            groupSize: int.parse(_groupSize.text),
                            timeBetweenGroups: double.parse(_delay.text),
                            zombies: _zombies,
                          );
                          Navigator.of(context).pop();
                        }
                      },
                      child: SizedBox(
                        width: double.infinity,
                        child: Center(
                          child: Padding(
                            padding: const EdgeInsets.symmetric(
                              vertical: 20.0,
                            ),
                            child: Text(
                              los.save,
                            ),
                          ),
                        ),
                      ),
                    ),
                  ],
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}

class LevelLawn extends StatefulWidget {
  const LevelLawn({
    super.key,
    required this.onTap,
  });

  final Future<void> Function(
    int row,
    int col,
    Map<int, MemoryImage> cellItems,
    int index,
  ) onTap;

  @override
  State<LevelLawn> createState() => _LevelLawnState();
}

class _LevelLawnState extends State<LevelLawn> {
  final int rows = 5;
  final int columns = 9;

  final Map<int, MemoryImage> cellItems = {};

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      height: 300,
      width: 500,
      child: GridView.builder(
        gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: columns,
          mainAxisSpacing: 2,
          crossAxisSpacing: 2,
        ),
        itemCount: rows * columns,
        itemBuilder: (context, index) {
          final row = index ~/ columns;
          final col = index % columns;
          return GestureDetector(
            onTap: () => widget.onTap(row, col, cellItems, index),
            child: Container(
              color: Colors.white,
              child: Center(
                child: cellItems.containsKey(index)
                    ? SizedBox(
                        width: 40,
                        height: 40,
                        child: Image(
                          image: cellItems[index]!,
                          width: 40,
                          height: 40,
                        ),
                      )
                    : const SizedBox.shrink(),
              ),
            ),
          );
        },
      ),
    );
  }
}
