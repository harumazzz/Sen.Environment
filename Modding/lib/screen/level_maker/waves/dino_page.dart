import 'package:flutter/material.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/model/wave.dart';
import 'package:path/path.dart' as p;
import 'package:sen/service/file_helper.dart';

class DinoPage extends StatefulWidget {
  const DinoPage({
    super.key,
    required this.index,
    required this.wave,
    required this.dino,
  });

  final int index;

  final DinoWave wave;

  final List<String> dino;

  @override
  State<DinoPage> createState() => _DinoPageState();
}

class _DinoPageState extends State<DinoPage> {
  late String _dinoType;
  late int _row;
  late List<_DinoWrapper> _dinos;

  @override
  void initState() {
    super.initState();
    _dinoType = widget.wave.dinoType;
    _row = widget.wave.row;
    _dinos = widget.dino
        .map(
          (e) => _DinoWrapper(
            dinoType: p.basenameWithoutExtension(e),
            image: MemoryImage(FileHelper.readBuffer(source: e)),
          ),
        )
        .toList();
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
      title: Text('${los.wave} ${widget.index}: ${los.dino_wave}'),
    );
  }

  Widget _buildBody(AppLocalizations los) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: SingleChildScrollView(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          mainAxisAlignment: MainAxisAlignment.start,
          spacing: 20.0,
          children: [
            Card(
              child: Padding(
                padding: const EdgeInsets.symmetric(
                  horizontal: 20.0,
                  vertical: 12.0,
                ),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  mainAxisAlignment: MainAxisAlignment.start,
                  spacing: 16.0,
                  children: [
                    Text(
                      los.entry,
                      style: Theme.of(context).textTheme.titleLarge?.copyWith(fontWeight: FontWeight.bold),
                    ),
                    _buildDinoTypeDropdown(los),
                    _buildRowDropdown(los),
                  ],
                ),
              ),
            ),
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
      row: _row,
      dinoType: _dinoType,
    );
    Navigator.of(context).pop();
  }

  Widget _buildDinoTypeDropdown(AppLocalizations los) {
    return Row(
      spacing: 20.0,
      children: [
        Text('${los.dino_type}:'),
        Expanded(
          child: DropdownButton<String>(
            value: _dinoType,
            items: _dinos
                .map<DropdownMenuItem<String>>(
                  (e) => DropdownMenuItem(
                    value: e.dinoType,
                    child: Row(
                      spacing: 5.0,
                      mainAxisSize: MainAxisSize.min,
                      crossAxisAlignment: CrossAxisAlignment.start,
                      mainAxisAlignment: MainAxisAlignment.start,
                      children: [
                        Image(
                          image: e.image,
                          width: 40,
                          height: 40,
                        ),
                        Text(e.dinoType),
                      ],
                    ),
                  ),
                )
                .toList(),
            onChanged: (String? value) {
              if (value != null) {
                setState(() {
                  _dinoType = value;
                });
              }
            },
          ),
        ),
      ],
    );
  }

  Widget _buildRowDropdown(AppLocalizations los) {
    return Row(
      spacing: 20.0,
      children: [
        Text('${los.row}:'),
        Expanded(
          child: DropdownButton<int>(
            value: _row,
            items: [1, 2, 3, 4, 5]
                .map<DropdownMenuItem<int>>(
                  (e) => DropdownMenuItem(
                    value: e,
                    child: Text(e.toString()),
                  ),
                )
                .toList(),
            onChanged: (int? value) {
              if (value != null) {
                setState(() {
                  _row = value;
                });
              }
            },
          ),
        ),
      ],
    );
  }
}

class _DinoWrapper {
  String dinoType;
  MemoryImage image;
  _DinoWrapper({
    required this.dinoType,
    required this.image,
  });
}
