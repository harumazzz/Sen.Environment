import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/extension/platform.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/model/item.dart';
import 'package:sen/screen/animation_viewer/animation_viewer.dart';
import 'package:sen/screen/home/configuration/javascript_category_configuration.dart';
import 'package:sen/screen/home/configuration/level_maker_configuration.dart';
import 'package:sen/screen/home/configuration/map_editor_configuration.dart';
import 'package:sen/screen/home/configuration/shell_configuration.dart';
import 'package:sen/screen/javascript_category/javascript_category.dart';
import 'package:sen/screen/level_maker/level_maker.dart';
import 'package:sen/screen/map_editor/map_editor.dart';
import 'package:sen/screen/shell/shell_screen.dart';
import 'package:sen/service/ui_helper.dart';
import 'package:sen/widget/animated_floating.dart';

class HomeScreen extends StatelessWidget {
  const HomeScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    final items = _buildItems(context, los);
    final settings = BlocProvider.of<SettingsCubit>(context, listen: true);
    if (CurrentPlatform.isDesktop) {
      return _buildGridView(context, items, settings, los);
    }
    return _buildListView(items, settings, los);
  }

  List<Item> _buildItems(BuildContext context, AppLocalizations los) {
    return [
      Item(
        title: los.shell,
        description: los.shell_description,
        icon: Icon(Symbols.terminal_rounded, size: 50, color: Colors.blueGrey.shade600),
        onWidget: () => const ShellScreen(),
        onSetting: () async => await _onLoadShellConfiguration(context),
      ),
      Item(
        title: los.js_execute,
        description: los.js_execute_description,
        icon: Icon(Symbols.javascript_rounded, size: 50, color: Colors.yellow.shade600),
        onWidget: () => const JavaScriptCategory(),
        onSetting: () async => await _showJsSettings(context),
      ),
      Item(
        title: los.animation_viewer,
        description: los.animation_viewer_description,
        icon: Icon(Symbols.animated_images, size: 50, color: Colors.green.shade700),
        onWidget: () => const AnimationViewer(),
      ),
      Item(
        title: los.level_maker,
        description: los.level_maker_description,
        icon: Icon(Symbols.build, size: 50, color: Colors.cyan.shade600),
        onWidget: () => const LevelMaker(),
        onSetting: () async => await _onLoadLevelMakerConfiguration(context),
      ),
      Item(
        title: los.map_editor,
        description: los.map_editor_description,
        icon: Icon(Symbols.map, size: 50, color: Colors.lightBlue.shade600),
        onWidget: () => const MapEditor(),
        onSetting: () async => await _onLoadMapEditorConfiguration(context),
      ),
    ];
  }

  Widget _buildGridView(
    BuildContext context,
    List<Item> items,
    SettingsCubit settings,
    AppLocalizations los,
  ) {
    final screenWidth = MediaQuery.of(context).size.width;
    const itemWidth = 250.0;
    final crossAxisCount = (screenWidth / itemWidth).floor();
    return GridView.builder(
      padding: const EdgeInsets.all(8),
      gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
        crossAxisCount: crossAxisCount,
        childAspectRatio: 1,
      ),
      itemCount: items.length,
      itemBuilder: (context, index) => _buildCard(context, items[index], settings, los),
    );
  }

  Widget _buildListView(
    List<Item> items,
    SettingsCubit settings,
    AppLocalizations los,
  ) {
    return ListView.builder(
      padding: const EdgeInsets.only(top: 12),
      itemCount: items.length,
      itemBuilder: (context, index) => _buildCard(context, items[index], settings, los),
    );
  }

  Widget _buildCard(
    BuildContext context,
    Item item,
    SettingsCubit settings,
    AppLocalizations los,
  ) {
    return Card(
      elevation: 4.0,
      margin: const EdgeInsets.all(8.0),
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10.0)),
      color: Theme.of(context).colorScheme.surfaceContainerHigh,
      child: InkWell(
        onTap: () => Navigator.of(context).push(MaterialPageRoute(builder: (context) => item.onWidget?.call())),
        borderRadius: BorderRadius.circular(10),
        child: Padding(
          padding: const EdgeInsets.all(12),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              CurrentPlatform.isDesktop ? AnimatedFloating(child: item.icon) : item.icon,
              const SizedBox(height: 8.0),
              Text(
                item.title,
                style: TextStyle(
                  fontSize: CurrentPlatform.isDesktop ? 16 : 14,
                  fontWeight: FontWeight.w600,
                ),
                textAlign: TextAlign.center,
                overflow: TextOverflow.ellipsis,
              ),
              const SizedBox(height: 6.0),
              Text(
                item.description,
                style: const TextStyle(fontSize: 12, color: Colors.grey),
                textAlign: TextAlign.center,
                overflow: TextOverflow.ellipsis,
                maxLines: 8,
              ),
              const SizedBox(height: 8.0),
              if (item.onSetting != null)
                IconButton(
                  icon: const Icon(Icons.settings, size: 20),
                  onPressed: item.onSetting,
                  tooltip: los.settings,
                ),
            ],
          ),
        ),
      ),
    );
  }

  Future<void> _showJsSettings(
    BuildContext context,
  ) async {
    final los = AppLocalizations.of(context)!;
    await UIHelper.showDetailDialog(
      context: context,
      title: Text(los.js_settings),
      content: const JavaScriptCategoryConfiguration(),
    );
  }

  Future<void> _onLoadLevelMakerConfiguration(
    BuildContext context,
  ) async {
    final los = AppLocalizations.of(context)!;
    await UIHelper.showDetailDialog(
      context: context,
      title: Text(los.level_maker),
      content: const LevelMakerConfiguration(),
    );
  }

  Future<void> _onLoadMapEditorConfiguration(
    BuildContext context,
  ) async {
    final los = AppLocalizations.of(context)!;
    await UIHelper.showDetailDialog(
      context: context,
      title: Text(los.map_editor),
      content: const MapEditorConfiguration(),
    );
  }

  Future<void> _onLoadShellConfiguration(
    BuildContext context,
  ) async {
    final los = AppLocalizations.of(context)!;
    await UIHelper.showDetailDialog(
      context: context,
      title: Text(los.shell_configuration),
      content: const ShellConfiguration(),
    );
  }
}
