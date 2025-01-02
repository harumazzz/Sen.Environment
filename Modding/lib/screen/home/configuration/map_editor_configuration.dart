import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/provider/map_provider.dart';
import 'package:sen/service/file_service.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class MapEditorConfiguration extends ConsumerStatefulWidget {
  const MapEditorConfiguration({super.key});

  @override
  ConsumerState<MapEditorConfiguration> createState() => _MapEditorConfigurationState();
}

class _MapEditorConfigurationState extends ConsumerState<MapEditorConfiguration> {
  late TextEditingController _resourceLocationController;

  @override
  void initState() {
    super.initState();
    _resourceLocationController = TextEditingController(text: '');
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  @override
  void dispose() {
    _resourceLocationController.dispose();
    super.dispose();
  }

  Future<void> _onValueChange() async {
    final value = _resourceLocationController.text;
    await ref.watch(mapProvider.notifier).setResourceLocation(value);
  }

  void _onChangeSetting(String? value) async {
    if (value == null) return;
    _resourceLocationController.text = value;
    await _onValueChange();
  }

  void _onUploadDirectory() async {
    var result = await FileService.uploadDirectory();
    if (result != null) {
      _resourceLocationController.text = result;
      await _onValueChange();
    }
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    if (ref.watch(mapProvider).isLoading) {
      return const CircularProgressIndicator.adaptive();
    } else {
      final value = ref.watch(mapProvider).resourceLocation;
      if (value != null) {
        _resourceLocationController.text = value;
      }
    }
    return Padding(
      padding: const EdgeInsets.symmetric(
        horizontal: 12.0,
        vertical: 8.0,
      ),
      child: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.start,
          crossAxisAlignment: CrossAxisAlignment.start,
          children: <Widget>[
            const SizedBox(height: 12.0),
            Text(
              los.settings,
              style: Theme.of(context).textTheme.titleLarge?.copyWith(fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 12.0),
            Row(
              children: [
                Text('${los.resource_location}: '),
                const SizedBox(width: 8.0),
                Expanded(
                  child: TextField(
                    controller: _resourceLocationController,
                    onChanged: _onChangeSetting,
                  ),
                ),
                const SizedBox(width: 4.0),
                IconButton(
                  onPressed: _onUploadDirectory,
                  icon: Tooltip(
                    message: los.upload_directory,
                    child: const Icon(Symbols.folder),
                  ),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
