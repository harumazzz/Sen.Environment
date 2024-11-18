import 'package:flutter/material.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/model/wave.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class FrostWindPage extends StatefulWidget {
  const FrostWindPage({
    super.key,
    required this.wave,
    required this.index,
  });

  final FrostWind wave;

  final int index;

  @override
  State<FrostWindPage> createState() => _FrostWindPageState();
}

class _FrostWindPageState extends State<FrostWindPage> {
  late FrostWind _frostWind;

  @override
  void initState() {
    super.initState();
    _frostWind = widget.wave.clone();
  }

  @override
  void dispose() {
    super.dispose();
  }

  void _onSave() async {
    widget.wave.replaceWith(_frostWind);
    await _onSaveFinish();
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  List<Widget> _buildFrostWind(
    AppLocalizations los,
  ) {
    makeDescription(Wind e) {
      return '${los.direction}: ${e.direction}, ${los.row}: ${e.row}';
    }

    return _frostWind.winds
        .asMap()
        .entries
        .map(
          (MapEntry<int, Wind> e) => Card(
            child: Padding(
              padding: const EdgeInsets.symmetric(
                horizontal: 12.0,
                vertical: 8.0,
              ),
              child: ListTile(
                leading: const Icon(Symbols.circle, color: Colors.cyan),
                title: Text('${e.key}'),
                subtitle: Text(makeDescription(e.value)),
                trailing: Row(
                  mainAxisSize: MainAxisSize.min,
                  children: [
                    IconButton(
                      onPressed: () => _onEdit(index: e.key),
                      icon: Tooltip(
                        message: los.edit,
                        child: const Icon(Symbols.edit),
                      ),
                    ),
                    const SizedBox(width: 15.0),
                    IconButton(
                      onPressed: () => _onDelete(index: e.key),
                      icon: Tooltip(
                        message: los.delete,
                        child: const Icon(Symbols.delete),
                      ),
                    ),
                  ],
                ),
              ),
            ),
          ),
        )
        .toList();
  }

  void _onDelete({
    required int index,
  }) {
    setState(() {
      _frostWind.winds.removeAt(index);
    });
  }

  Future<void> _onSaveFinish() async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.done),
        content: Text(los.save_finish),
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

  void _onAdd() async {
    final los = AppLocalizations.of(context)!;
    final state = <String, void Function()?>{
      'onAdd': null,
    };
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.add_new_frost_wind),
        content: _AddFrostWind(
          frostWind: _frostWind,
          map: state,
        ),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop();
            },
            child: Text(los.close),
          ),
          TextButton(
            onPressed: () {
              state['onAdd']?.call();
              Navigator.of(context).pop();
              setState(() {});
            },
            child: Text(los.add),
          ),
        ],
      ),
    );
  }

  void _onEdit({
    required int index,
  }) async {
    final los = AppLocalizations.of(context)!;
    final state = <String, void Function()?>{
      'onEdit': null,
    };
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.edit),
        content: _EditFrostWind(
          frostWind: _frostWind,
          map: state,
          index: index,
        ),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop();
            },
            child: Text(los.close),
          ),
          TextButton(
            onPressed: () {
              state['onEdit']?.call();
              Navigator.of(context).pop();
              setState(() {});
            },
            child: Text(los.edit),
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
        title: Text('${los.wave} ${widget.index}: ${los.frost_wind}'),
        actions: [
          Tooltip(
            message: los.save,
            child: IconButton(
              onPressed: _onSave,
              icon: const Icon(Symbols.save),
            ),
          ),
        ],
      ),
      body: Padding(
        padding: const EdgeInsets.all(8.0),
        child: SingleChildScrollView(
          child: Column(
            children: _buildFrostWind(los),
          ),
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: _onAdd,
        child: Tooltip(
          message: los.add_new_frost_wind,
          child: const Icon(Symbols.add),
        ),
      ),
    );
  }
}

class _DirectionWrapper {
  String direction;
  String locale;
  _DirectionWrapper({
    required this.direction,
    required this.locale,
  });
}

class _AddFrostWind extends StatefulWidget {
  const _AddFrostWind({
    required this.frostWind,
    required this.map,
  });

  final FrostWind frostWind;
  final Map<String, void Function()?> map;

  @override
  State<_AddFrostWind> createState() => __AddFrostWindState();
}

class __AddFrostWindState extends State<_AddFrostWind> {
  late final List<_DirectionWrapper> _directions;
  final List<int> _rows = [1, 2, 3, 4, 5];

  late String _direction;

  late int _row;

  @override
  void initState() {
    _direction = 'left';
    _row = 1;
    widget.map['onAdd'] = () {
      widget.frostWind.winds.add(Wind(row: _row, direction: _direction));
    };
    super.initState();
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
    _directions = [
      _DirectionWrapper(
        direction: 'left',
        locale: AppLocalizations.of(context)!.left,
      ),
      _DirectionWrapper(
        direction: 'right',
        locale: AppLocalizations.of(context)!.right,
      ),
    ];
  }

  @override
  void dispose() {
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Column(
      mainAxisSize: MainAxisSize.min,
      mainAxisAlignment: MainAxisAlignment.start,
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Row(
          children: [
            Text(los.direction),
            const SizedBox(width: 15.0),
            Expanded(
              child: DropdownButton<String>(
                items: _directions
                    .map<DropdownMenuItem<String>>(
                      (e) => DropdownMenuItem<String>(
                        value: e.direction,
                        child: Text(e.locale),
                      ),
                    )
                    .toList(),
                value: _direction,
                onChanged: (String? value) {
                  if (value == null) return;
                  setState(() {
                    _direction = value;
                  });
                },
              ),
            ),
          ],
        ),
        Row(
          children: [
            Text(los.row),
            const SizedBox(width: 15.0),
            Expanded(
              child: DropdownButton<int>(
                items: _rows
                    .map<DropdownMenuItem<int>>(
                      (e) => DropdownMenuItem<int>(
                        value: e,
                        child: Text(e.toString()),
                      ),
                    )
                    .toList(),
                value: _row,
                onChanged: (int? value) {
                  if (value == null) return;
                  setState(() {
                    _row = value;
                  });
                },
              ),
            ),
          ],
        ),
      ],
    );
  }
}

class _EditFrostWind extends StatefulWidget {
  const _EditFrostWind({
    required this.frostWind,
    required this.map,
    required this.index,
  });

  final FrostWind frostWind;

  final int index;

  final Map<String, void Function()?> map;

  @override
  State<_EditFrostWind> createState() => __EditFrostWindState();
}

class __EditFrostWindState extends State<_EditFrostWind> {
  late final List<_DirectionWrapper> _directions;
  final List<int> _rows = [1, 2, 3, 4, 5];

  late Wind wind;

  late String _direction;

  late int _row;

  @override
  void initState() {
    wind = widget.frostWind.winds[widget.index];
    _direction = wind.direction;
    _row = wind.row;
    widget.map['onEdit'] = () {
      widget.frostWind.winds[widget.index] =
          Wind(row: _row, direction: _direction);
    };
    super.initState();
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
    _directions = [
      _DirectionWrapper(
        direction: 'left',
        locale: AppLocalizations.of(context)!.left,
      ),
      _DirectionWrapper(
        direction: 'right',
        locale: AppLocalizations.of(context)!.right,
      ),
    ];
  }

  @override
  void dispose() {
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Column(
      mainAxisSize: MainAxisSize.min,
      mainAxisAlignment: MainAxisAlignment.start,
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Row(
          children: [
            Text(los.direction),
            const SizedBox(width: 15.0),
            Expanded(
              child: DropdownButton<String>(
                items: _directions
                    .map<DropdownMenuItem<String>>(
                      (e) => DropdownMenuItem<String>(
                        value: e.direction,
                        child: Text(e.locale),
                      ),
                    )
                    .toList(),
                value: _direction,
                onChanged: (String? value) {
                  if (value == null) return;
                  setState(() {
                    _direction = value;
                  });
                },
              ),
            ),
          ],
        ),
        Row(
          children: [
            Text(los.row),
            const SizedBox(width: 15.0),
            Expanded(
              child: DropdownButton<int>(
                items: _rows
                    .map<DropdownMenuItem<int>>(
                      (e) => DropdownMenuItem<int>(
                        value: e,
                        child: Text(e.toString()),
                      ),
                    )
                    .toList(),
                value: _row,
                onChanged: (int? value) {
                  if (value == null) return;
                  setState(() {
                    _row = value;
                  });
                },
              ),
            ),
          ],
        ),
      ],
    );
  }
}
