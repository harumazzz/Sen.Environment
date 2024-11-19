import 'dart:math';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:sen/model/wave.dart';
import 'package:sen/screen/level_maker/zombie_suggestion.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/service/file_service.dart';

class GroundPage extends StatefulWidget {
  const GroundPage({
    super.key,
    required this.wave,
    required this.index,
    required this.zombies,
    required this.resource,
  });

  final GroundSpawn wave;
  final List<String> zombies;
  final String resource;
  final int index;

  @override
  State<GroundPage> createState() => _GroundPageState();
}

class _GroundPageState extends State<GroundPage> {
  late TextEditingController _xStart;
  late TextEditingController _xEnd;
  late TextEditingController _plantfood;
  late GlobalKey<FormState> _formKey;
  late List<Spawn> _zombies;
  late TextEditingController _zombie;

  late Map<int, MemoryImage> _cellItems;

  @override
  void initState() {
    _xStart = TextEditingController(text: widget.wave.columnStart.toString());
    _xEnd = TextEditingController(text: widget.wave.columnEnd.toString());
    _plantfood =
        TextEditingController(text: widget.wave.additionalPlantFood.toString());
    _zombie = TextEditingController();
    _formKey = GlobalKey<FormState>();
    _zombies = [...widget.wave.zombies];
    _cellItems = {};
    _initCellItems();
    super.initState();
  }

  int get _columnStart => int.parse(_xStart.text);

  set _columnStart(int value) {
    _xStart.text = value.toString();
  }

  int get _columnEnd => int.parse(_xEnd.text);

  set _columnEnd(int value) {
    _xEnd.text = value.toString();
  }

  void _initCellItems() {
    _cellItems = {};
    final random = Random();
    for (var zombie in _zombies) {
      final col = _columnEnd == _columnStart
          ? _columnEnd
          : random.nextInt((_columnEnd - _columnStart)) +
              int.parse(_xStart.text);
      final row = zombie.row;
      final index = row * 9 + col;
      final state = '${widget.resource}/zombie/${zombie.typename}.png';
      _cellItems[index] = MemoryImage(FileService.readBuffer(source: state));
    }
  }

  @override
  void dispose() {
    _xStart.dispose();
    _xEnd.dispose();
    _plantfood.dispose();
    _zombie.dispose();
    super.dispose();
  }

  Widget _buildTextField({
    required TextEditingController controller,
    required String label,
    bool enabled = true,
    TextInputType keyboardType = TextInputType.text,
    List<TextInputFormatter>? inputFormatters,
  }) {
    return Padding(
      padding: const EdgeInsets.symmetric(vertical: 8.0),
      child: TextFormField(
        controller: controller,
        decoration: InputDecoration(
          labelText: label,
        ),
        enabled: enabled,
        keyboardType: keyboardType,
        inputFormatters: inputFormatters,
      ),
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
            if (_columnEnd == 0 && _columnStart == 0) {
              _columnEnd = col;
              _columnStart = col;
            }
            if (col > _columnEnd) {
              _columnEnd = col;
            } else if (col < _columnStart) {
              _columnStart = col;
            }
            if (_columnEnd < _columnStart) {
              _columnEnd = _columnStart;
            }
            _zombies.add(Spawn(row: row, typename: value));
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

  Widget _buildCard(String title, List<Widget> children) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(12.0),
        child: Column(
          children: [
            Text(
              title,
              style: Theme.of(context).textTheme.titleLarge?.copyWith(
                    fontWeight: FontWeight.bold,
                  ),
            ),
            const SizedBox(height: 20.0),
            ...children,
          ],
        ),
      ),
    );
  }

  Widget _buildSaveButton() {
    return ElevatedButton(
      onPressed: () {
        if (_formKey.currentState!.validate()) {
          widget.wave.replaceWith(
            additionalPlantFood: int.parse(_plantfood.text),
            columnStart: _columnStart,
            columnEnd: _columnEnd,
            zombies: _zombies,
          );
          Navigator.of(context).pop();
        }
      },
      child: Padding(
        padding: const EdgeInsets.symmetric(vertical: 20.0),
        child: Center(child: Text(AppLocalizations.of(context)!.save)),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Scaffold(
      appBar: AppBar(
        title: Text('${los.wave} ${widget.index}: ${los.ground_spawn}'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(12.0),
        child: Form(
          key: _formKey,
          child: SingleChildScrollView(
            child: Column(
              children: [
                _buildCard(
                  los.entry,
                  [
                    _buildTextField(
                      controller: _xStart,
                      label: los.column_start,
                      enabled: false,
                    ),
                    _buildTextField(
                      controller: _xEnd,
                      label: los.column_end,
                      enabled: false,
                    ),
                    _buildTextField(
                      controller: _plantfood,
                      label: los.additional_plantfood,
                      keyboardType: TextInputType.number,
                      inputFormatters: [
                        FilteringTextInputFormatter.digitsOnly,
                      ],
                    ),
                    const SizedBox(height: 10.0),
                    Text(
                      los.zombie,
                      style: Theme.of(context).textTheme.titleLarge?.copyWith(
                            fontWeight: FontWeight.bold,
                          ),
                    ),
                    const SizedBox(height: 20.0),
                    _LevelLawn(
                      onTap: _onLevelLawnSpawn,
                      cellItems: _cellItems,
                    ),
                  ],
                ),
                const SizedBox(height: 20.0),
                _buildSaveButton(),
              ],
            ),
          ),
        ),
      ),
    );
  }
}

class _LevelLawn extends StatefulWidget {
  const _LevelLawn({
    required this.onTap,
    required this.cellItems,
  });

  final Map<int, MemoryImage> cellItems;

  final Future<void> Function(
    int row,
    int col,
    Map<int, MemoryImage> cellItems,
    int index,
  ) onTap;

  @override
  State<_LevelLawn> createState() => __LevelLawnState();
}

class __LevelLawnState extends State<_LevelLawn> {
  final int rows = 5;
  final int columns = 9;

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
            onTap: () => widget.onTap(row, col, widget.cellItems, index),
            child: Container(
              color: Colors.white,
              child: Center(
                child: widget.cellItems.containsKey(index)
                    ? SizedBox(
                        width: 40,
                        height: 40,
                        child: Image(
                          image: widget.cellItems[index]!,
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
