import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:sen/provider/map_provider.dart';
import 'package:sen/screen/map_editor/viewmodel/map_manager.dart';
import 'package:sen/screen/map_editor/viewmodel/resource_manager.dart';
import 'package:sen/screen/map_editor/viewmodel/state_manager.dart';
import 'package:sen/screen/map_editor/view/map_stage.dart';
import 'package:sen/screen/map_editor/view/section.dart';
import 'package:sen/screen/map_editor/view/toolbar.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class MapEditor extends ConsumerStatefulWidget {
  const MapEditor({super.key});

  @override
  ConsumerState<MapEditor> createState() => _MapEditorState();
}

class _MapEditorState extends ConsumerState<MapEditor> {
  bool _isLoaded = false;

  Future<void> _loadResources(BuildContext context, WidgetRef ref) async {
    final los = AppLocalizations.of(context)!;
    await Future.doWhile(() async {
      if (ref.read(mapProvider).isLoading) {
        await Future.delayed(const Duration(milliseconds: 100));
        return true;
      }
      return false;
    });
    final setting = ref.watch(mapProvider).resourceLocation;
    if (setting == null || setting.isEmpty) {
      throw Exception(los.please_configure_resource_location);
    }
    StateManager.initState(ref);
    ResourceLoader.loadConfig('$setting/config.json');
    ResourceManager.resourcePath = setting;
    await ResourceManager.load(los);
    MapManager.init();
    CameraView.setCameraViewInitOffset();
    setState(() {
      _isLoaded = true;
    });
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    ref.listen<String>(snackBarProvider, (previous, next) {
      if (next != 'null') {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text(next),
            width: 400,
            behavior: SnackBarBehavior.floating,
            duration: const Duration(seconds: 2),
          ),
        );
      }
    });
    return Scaffold(
      appBar: AppBar(
        forceMaterialTransparency: true,
        leading: IconButton(
          icon: const Icon(Icons.arrow_back),
          onPressed: () {
            Navigator.of(context).pop();
            StateManager.dispose();
            _isLoaded = false;
          },
        ),
        title: Text(AppLocalizations.of(context)!.map_editor),
      ),
      body: _isLoaded
          ? const Row(
              children: [
                SizedBox(
                    width: 60,
                    child: Padding(
                      padding: EdgeInsets.only(top: 6, left: 6),
                      child: ToolBarWidget(),
                    )),
                Expanded(
                    child: Column(
                  mainAxisSize: MainAxisSize.max,
                  children: [
                    Expanded(
                      child: SizedBox(
                          width: double.infinity,
                          child: Card.outlined(
                            clipBehavior: Clip.antiAlias,
                            shape: RoundedRectangleBorder(
                                borderRadius: BorderRadius.all(Radius.circular(16)),
                                side: BorderSide(color: Color.fromRGBO(0, 0, 0, 0.3), width: 2)),
                            child: Stack(
                              fit: StackFit.passthrough,
                              children: [
                                ToolBarHotkeys(
                                  child: MapStage(),
                                ),
                                ShopWidget(),
                                PaletteWidget(),
                                EditorSettingWidget()
                              ],
                            ),
                          )),
                    ),
                    Row(
                      children: [
                        Expanded(
                            child: SizedBox(
                          height: 60,
                          child: SectionWidget(),
                        )),
                        SizedBox(
                          width: 120,
                          height: 60,
                          child: ExtensionWidget(),
                        ),
                      ],
                    )
                  ],
                )),
                SizedBox(
                  width: 280,
                  child: Card(
                      margin: EdgeInsets.only(top: 4, left: 4, right: 8, bottom: 12),
                      shadowColor: Colors.transparent,
                      child: Padding(
                        padding: EdgeInsets.all(8),
                        child: OptionWidget(),
                      )),
                ),
              ],
            )
          : FutureBuilder<void>(
              future: _loadResources(context, ref),
              builder: (context, snapshot) {
                if (snapshot.connectionState == ConnectionState.waiting) {
                  return const Center(
                    child: CircularProgressIndicator(),
                  );
                } else if (snapshot.hasError) {
                  return Center(
                    child: Text(
                      '${los.loading_error}: ${snapshot.error}',
                      style: Theme.of(context).textTheme.bodyLarge,
                    ),
                  );
                } else {
                  return const SizedBox.shrink();
                }
              },
            ),
    );
  }
}

class SnackBarNotifier extends StateNotifier<String> {
  SnackBarNotifier() : super('null');

  void showSnackBar(String message) {
    state = message;
    Future.delayed(const Duration(milliseconds: 500), () {
      state = 'null';
    });
  }
}

final snackBarProvider = StateNotifierProvider<SnackBarNotifier, String>(
  (ref) => SnackBarNotifier(),
);
