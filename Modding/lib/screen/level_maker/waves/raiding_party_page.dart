import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:sen/model/wave.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class RaidingPartyPage extends StatefulWidget {
  const RaidingPartyPage({
    super.key,
    required this.wave,
    required this.index,
  });

  final RaidingParty wave;

  final int index;

  @override
  State<RaidingPartyPage> createState() => _RaidingPartyPageState();
}

class _RaidingPartyPageState extends State<RaidingPartyPage> {
  late GlobalKey<FormState> _formKey;

  late TextEditingController _countController;
  late TextEditingController _groupSizeController;
  late TextEditingController _delayBetweenGroupController;

  @override
  void initState() {
    super.initState();
    _formKey = GlobalKey<FormState>();
    _countController = TextEditingController(
      text: widget.wave.count.toString(),
    );
    _groupSizeController = TextEditingController(
      text: widget.wave.groupSize.toString(),
    );
    _delayBetweenGroupController = TextEditingController(
      text: widget.wave.delayBetweenGroup.toString(),
    );
  }

  @override
  void dispose() {
    _countController.dispose();
    _groupSizeController.dispose();
    _delayBetweenGroupController.dispose();
    super.dispose();
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  Widget _buildFormField({
    required String label,
    required TextEditingController controller,
    required TextInputType keyboardType,
    required List<TextInputFormatter> inputFormatters,
  }) {
    return TextFormField(
      controller: controller,
      decoration: InputDecoration(labelText: label),
      keyboardType: keyboardType,
      inputFormatters: inputFormatters,
      validator: (String? value) {
        if (value == null) {
          return AppLocalizations.of(context)!.value_cannot_be_null;
        }
        if (value.isEmpty) {
          return AppLocalizations.of(context)!.value_cannot_be_empty;
        }
        return null;
      },
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
          horizontal: 20.0,
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
        count: int.parse(_countController.text),
        delayBetweenGroup: double.parse(_delayBetweenGroupController.text),
        groupSize: int.parse(_groupSizeController.text),
      );
      Navigator.of(context).pop();
    }
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Scaffold(
      appBar: AppBar(
        title: Text('${los.wave} ${widget.index}: ${los.raiding_party}'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(8.0),
        child: Form(
          key: _formKey,
          child: SingleChildScrollView(
            child: Column(
              children: [
                _buildCard(
                  title: los.entry,
                  children: [
                    _buildFormField(
                      label: los.swashbuckler_count,
                      controller: _countController,
                      keyboardType: TextInputType.number,
                      inputFormatters: [FilteringTextInputFormatter.digitsOnly],
                    ),
                    const SizedBox(height: 10.0),
                    _buildFormField(
                      label: los.group_size,
                      controller: _groupSizeController,
                      keyboardType: TextInputType.number,
                      inputFormatters: [FilteringTextInputFormatter.digitsOnly],
                    ),
                    const SizedBox(height: 10.0),
                    _buildFormField(
                      label: los.delay_between_groups,
                      controller: _delayBetweenGroupController,
                      keyboardType:
                          const TextInputType.numberWithOptions(decimal: true),
                      inputFormatters: [
                        FilteringTextInputFormatter.allow(RegExp(r'^\d*\.?\d*'))
                      ],
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
