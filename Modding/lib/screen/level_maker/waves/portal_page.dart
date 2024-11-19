import 'package:flutter/material.dart';
import 'package:sen/model/wave.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class PortalPage extends StatefulWidget {
  const PortalPage({
    super.key,
    required this.index,
    required this.wave,
    required this.portal,
  });

  final int index;

  final PortalWave wave;

  final List<String> portal;

  @override
  State<PortalPage> createState() => _PortalPageState();
}

class _PortalPageState extends State<PortalPage> {
  late TextEditingController _columnController;
  late TextEditingController _rowController;
  late TextEditingController _typeController;

  late Map<int, String> _cells;

  @override
  void initState() {
    super.initState();
    _columnController = TextEditingController(
      text: widget.wave.column.toString(),
    );
    _rowController = TextEditingController(
      text: widget.wave.row.toString(),
    );
    _typeController = TextEditingController(
      text: widget.wave.type,
    );
    _cells = <int, String>{};
    _cells[widget.wave.row * 9 + widget.wave.column] = widget.wave.type;
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  @override
  void dispose() {
    _columnController.dispose();
    _rowController.dispose();
    _typeController.dispose();
    super.dispose();
  }

  AppBar _buildAppBar(AppLocalizations los) {
    return AppBar(
      title: Text('${los.wave} ${widget.index}: ${los.portal_wave}'),
    );
  }

  Widget _buildMovableRow({
    required String title,
    required TextEditingController controller,
  }) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      mainAxisAlignment: MainAxisAlignment.start,
      children: [
        Row(
          children: [
            Text('$title:'),
            const SizedBox(width: 12.0),
            Expanded(
              child: TextField(
                controller: controller,
                enabled: false,
              ),
            ),
          ],
        ),
        const SizedBox(height: 8.0),
      ],
    );
  }

  Widget _buildEntry(AppLocalizations los) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.symmetric(
          horizontal: 20.0,
          vertical: 12.0,
        ),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          mainAxisAlignment: MainAxisAlignment.start,
          children: [
            Text(
              los.entry,
              style: Theme.of(context)
                  .textTheme
                  .titleLarge
                  ?.copyWith(fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 20.0),
            _buildMovableRow(
              title: los.portal_column,
              controller: _columnController,
            ),
            _buildMovableRow(
              title: los.portal_row,
              controller: _rowController,
            ),
            _buildMovableRow(
              title: los.portal_type,
              controller: _typeController,
            ),
          ],
        ),
      ),
    );
  }

  Future<void> _onPickPortal() async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.pick_a_portal),
        content: SingleChildScrollView(
          child: StatefulBuilder(
            builder: (context, setState) {
              return DropdownButton<String>(
                items: widget.portal
                    .map<DropdownMenuItem<String>>(
                      (e) => DropdownMenuItem<String>(
                        value: e,
                        child: Text(e),
                      ),
                    )
                    .toList(),
                value: _typeController.text,
                onChanged: (String? value) {
                  if (value == null) return;
                  setState(() {
                    _typeController.text = value;
                  });
                },
              );
            },
          ),
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

  Widget _buildLawn() {
    return _LevelLawn(
      cellItems: _cells,
      onTap: (row, col, cellItems, index) async {
        await _onPickPortal();
        _rowController.text = row.toString();
        _columnController.text = col.toString();
        _cells = {};
        _cells[index] = _typeController.text;
        setState(() {});
      },
    );
  }

  Widget _buildBody(AppLocalizations los) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: SingleChildScrollView(
        child: Column(
          children: [
            _buildEntry(los),
            const SizedBox(height: 20.0),
            _buildLawn(),
            const SizedBox(height: 20.0),
            _buildSaveButton(los),
          ],
        ),
      ),
    );
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
    widget.wave.replaceWith(
      column: int.parse(_columnController.text),
      row: int.parse(_rowController.text),
      type: _typeController.text,
    );
    Navigator.of(context).pop();
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Scaffold(
      appBar: _buildAppBar(los),
      body: _buildBody(los),
    );
  }
}

class _LevelLawn extends StatefulWidget {
  const _LevelLawn({
    required this.onTap,
    required this.cellItems,
  });

  final Map<int, String> cellItems;

  final Future<void> Function(
    int row,
    int col,
    Map<int, String> cellItems,
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
              decoration: BoxDecoration(
                border: Border.all(
                  color: Theme.of(context).brightness == Brightness.light
                      ? Colors.black
                      : Colors.white,
                  width: 1.0,
                ),
                color: Colors.white,
              ),
              child: Center(
                child: widget.cellItems.containsKey(index)
                    ? SizedBox(
                        width: 40,
                        height: 40,
                        child: Text(widget.cellItems[index]!),
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
