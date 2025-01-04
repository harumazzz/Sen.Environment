import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:sen/model/script.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/javascript_category/javascript_card.dart';
import 'package:sen/service/file_service.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class JavaScriptCategory extends ConsumerWidget {
  const JavaScriptCategory({
    super.key,
  });

  Future<ScriptData?> _loadData(WidgetRef ref) async {
    final scriptPath = '${ref.read(settingProvider).toolChain}/Script/Helper/script.json';
    if (FileService.isFile(scriptPath)) {
      final json = await FileService.readJsonAsync(source: scriptPath);
      final data = ScriptData.fromJson(json);
      return data;
    }
    return null;
  }

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final los = AppLocalizations.of(context)!;
    return Scaffold(
      appBar: AppBar(
        title: Text(los.js_page),
      ),
      body: Padding(
        padding: const EdgeInsets.all(8.0),
        child: FutureBuilder(
          future: _loadData(ref),
          builder: (context, snapshot) {
            if (snapshot.connectionState == ConnectionState.done) {
              if (snapshot.hasData && snapshot.data != null) {
                final data = snapshot.data as ScriptData;
                return ListView.builder(
                  itemCount: data.data.length,
                  itemBuilder: (context, index) {
                    final script = data.data[index];
                    return JavaScriptCard(
                      item: script,
                      toolChain: ref.read(settingProvider).toolChain,
                    );
                  },
                );
              } else {
                return Center(
                  child: Text(los.script_not_found),
                );
              }
            } else {
              return const Center(
                child: CircularProgressIndicator.adaptive(),
              );
            }
          },
        ),
      ),
    );
  }
}
