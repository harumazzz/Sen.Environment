import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/model/wave.dart';

// TODO : Find the grid items, do the level lawn

class NecromancyPage extends StatefulWidget {
  const NecromancyPage({
    super.key,
    required this.index,
    required this.wave,
    required this.zombies,
    required this.resource,
  });

  final int index;

  final Necromancy wave;
  final List<String> zombies;
  final String resource;

  @override
  State<NecromancyPage> createState() => _NecromancyPageState();
}

class _NecromancyPageState extends State<NecromancyPage> {
  late GlobalKey<FormState> _formKey;
  late TextEditingController _messageController;
  late TextEditingController _spawnWaitTimeController;
  late TextEditingController _additionalPlantFoodController;
  late bool _suppressActionIfNoGridItemsFound;
  late List<String> _zombies;
  late List<String> _gridTypes;

  @override
  void initState() {
    super.initState();
    _formKey = GlobalKey<FormState>();
    _messageController = TextEditingController(text: widget.wave.message);
    _spawnWaitTimeController = TextEditingController(
      text: widget.wave.spawnWaitTime.toString(),
    );
    _additionalPlantFoodController = TextEditingController(
      text: widget.wave.additionalPlantFood.toString(),
    );
    _suppressActionIfNoGridItemsFound =
        widget.wave.suppressActionIfNoGridItemsFound;
    _zombies = [...widget.wave.zombies];
    _gridTypes = [...widget.wave.gridTypes];
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  @override
  void dispose() {
    _messageController.dispose();
    _spawnWaitTimeController.dispose();
    _additionalPlantFoodController.dispose();
    super.dispose();
  }

  AppBar _buildAppBar(AppLocalizations los) {
    return AppBar(
      title: Text('${los.wave} ${widget.index}: ${los.necromancy}'),
    );
  }

  Widget _buildValidateFormBody(AppLocalizations los) {
    return Column(
      children: [
        Row(
          children: [
            Expanded(
              child: TextField(
                controller: _messageController,
                decoration: InputDecoration(labelText: los.start_message),
              ),
            ),
          ],
        ),
        const SizedBox(height: 12.0),
        Row(
          children: [
            Expanded(
              child: TextFormField(
                controller: _spawnWaitTimeController,
                decoration: InputDecoration(
                  labelText: los.zombie_spawn_wait_time,
                ),
                keyboardType: TextInputType.number,
                inputFormatters: [
                  FilteringTextInputFormatter.digitsOnly,
                ],
              ),
            ),
          ],
        ),
        const SizedBox(height: 12.0),
        Row(
          children: [
            Expanded(
              child: TextFormField(
                controller: _additionalPlantFoodController,
                decoration: InputDecoration(
                  labelText: los.additional_plantfood,
                ),
                keyboardType: TextInputType.number,
                inputFormatters: [
                  FilteringTextInputFormatter.digitsOnly,
                ],
              ),
            ),
          ],
        ),
        const SizedBox(height: 12.0),
        Row(
          crossAxisAlignment: CrossAxisAlignment.start,
          mainAxisAlignment: MainAxisAlignment.start,
          children: [
            Padding(
              padding: const EdgeInsets.only(top: 8.0),
              child: Text(los.suppress_action_if_no_grid_items_found),
            ),
            Expanded(
              child: Switch(
                value: _suppressActionIfNoGridItemsFound,
                onChanged: (bool? value) {
                  if (value == null) return;
                  setState(() {
                    _suppressActionIfNoGridItemsFound = value;
                  });
                },
              ),
            )
          ],
        )
      ],
    );
  }

  Widget _buildBody(AppLocalizations los) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: Form(
        key: _formKey,
        child: SingleChildScrollView(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            mainAxisAlignment: MainAxisAlignment.start,
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
                    children: [
                      Text(
                        los.entry,
                        style: Theme.of(context)
                            .textTheme
                            .titleLarge
                            ?.copyWith(fontWeight: FontWeight.bold),
                      ),
                      const SizedBox(height: 16.0),
                      _buildValidateFormBody(los),
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
      additionalPlantFood: int.parse(_additionalPlantFoodController.text),
      message: _messageController.text,
      spawnWaitTime: int.parse(_spawnWaitTimeController.text),
      suppressActionIfNoGridItemsFound: _suppressActionIfNoGridItemsFound,
      zombies: _zombies,
      gridTypes: _gridTypes,
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
