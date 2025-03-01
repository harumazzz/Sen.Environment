import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/extension/context.dart';
import 'package:sen/extension/platform.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/model/item.dart';
import 'package:sen/screen/animation_viewer/animation_viewer.dart';
import 'package:sen/screen/home_screen/configuration/javascript_category_configuration.dart';
import 'package:sen/screen/home_screen/configuration/level_maker_configuration.dart';
import 'package:sen/screen/home_screen/configuration/map_editor_configuration.dart';
import 'package:sen/screen/home_screen/configuration/shell_configuration.dart';
import 'package:sen/screen/javascript_category/javascript_category.dart';
import 'package:sen/screen/level_maker/level_maker.dart';
import 'package:sen/screen/map_editor/map_editor.dart';
import 'package:sen/screen/shell_screen/shell_screen.dart';
import 'package:sen/service/ui_helper.dart';
import 'package:sen/widget/animated_floating.dart';

class HomeScreen extends StatelessWidget {
  const HomeScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final items = _buildItems(context);
    final settings = BlocProvider.of<SettingsCubit>(context, listen: true);
    if (CurrentPlatform.isDesktop) {
      return _buildGridView(context, items, settings);
    }
    return _buildListView(items, settings);
  }

  List<Item> _buildItems(
    BuildContext context,
  ) {
    final los = context.los;
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
        onSetting: () async => throw Exception('TODO'),
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

  int _calculateCrossAxisCount(BuildContext context) {
    final width = MediaQuery.of(context).size.width;
    if (width > 1800) return 7;
    if (width > 1400) return 5;
    if (width > 1000) return 4;
    if (width > 700) return 3;
    return 2;
  }

  double _calculateChildAspectRatio(BuildContext context, int crossAxisCount) {
    final screenWidth = MediaQuery.of(context).size.width;
    final screenHeight = MediaQuery.of(context).size.height;
    final spacing = 4.0 * (crossAxisCount - 1);
    final availableWidth = (screenWidth - 16 - spacing) / crossAxisCount;
    final estimatedHeight = availableWidth * 1.1;
    final maxHeight = screenHeight * 0.38;
    final temporaryHeight = estimatedHeight.clamp(120, maxHeight);
    return availableWidth / temporaryHeight;
  }

  Widget _buildGridView(
    BuildContext context,
    List<Item> items,
    SettingsCubit settings,
  ) {
    final crossAxisCount = _calculateCrossAxisCount(context);
    return GridView.builder(
      padding: const EdgeInsets.all(8.0),
      gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
        crossAxisCount: crossAxisCount,
        crossAxisSpacing: 4.0,
        mainAxisSpacing: 4.0,
        childAspectRatio: _calculateChildAspectRatio(context, crossAxisCount),
      ),
      itemCount: items.length,
      itemBuilder: (context, index) => _buildCard(context, items[index], settings),
    );
  }

  Widget _buildListView(
    List<Item> items,
    SettingsCubit settings,
  ) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: ListView.builder(
        padding: const EdgeInsets.only(top: 12.0),
        itemCount: items.length,
        itemBuilder: (context, index) => _buildCard(context, items[index], settings),
      ),
    );
  }

  Widget _buildCard(
    BuildContext context,
    Item item,
    SettingsCubit settings,
  ) {
    final isDesktop = CurrentPlatform.isDesktop;
    return Card(
      elevation: 4.0,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16.0)),
      color: Theme.of(context).colorScheme.surfaceContainerHigh,
      child: InkWell(
        onTap: () => Navigator.of(context).push(
          MaterialPageRoute(builder: (context) => item.onWidget?.call()),
        ),
        borderRadius: BorderRadius.circular(16.0),
        child: Padding(
          padding: _paddingOf(),
          child: isDesktop ? _buildDesktopCardContent(context, item) : _buildMobileCardContent(context, item),
        ),
      ),
    );
  }

  EdgeInsetsGeometry _paddingOf() {
    if (CurrentPlatform.isDesktop) {
      return const EdgeInsets.symmetric(horizontal: 12, vertical: 16);
    }
    return const EdgeInsets.symmetric(horizontal: 8.0, vertical: 12.0);
  }

  Widget _buildDesktopCardContent(
    BuildContext context,
    Item item,
  ) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.center,
      spacing: 15.0,
      children: [
        Align(
          alignment: Alignment.topRight,
          child: item.onSetting != null
              ? IconButton(
                  icon: const Icon(Symbols.settings, size: 24.0),
                  onPressed: item.onSetting,
                  tooltip: context.los.settings,
                )
              : const SizedBox.shrink(),
        ),
        AnimatedFloating(child: item.icon),
        Text(
          item.title,
          style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
          textAlign: TextAlign.center,
        ),
        Padding(
          padding: const EdgeInsets.symmetric(horizontal: 12),
          child: Text(
            item.description,
            style: const TextStyle(fontSize: 14, color: Colors.grey),
            textAlign: TextAlign.center,
            maxLines: 3,
            overflow: TextOverflow.ellipsis,
          ),
        ),
      ],
    );
  }

  Widget _buildMobileCardContent(
    BuildContext context,
    Item item,
  ) {
    final los = context.los;
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 6.0, vertical: 4.0),
      child: Row(
        crossAxisAlignment: CrossAxisAlignment.center,
        children: [
          item.icon,
          const SizedBox(width: 12.0),
          Expanded(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  item.title,
                  style: const TextStyle(fontSize: 15, fontWeight: FontWeight.w600),
                  overflow: TextOverflow.ellipsis,
                  maxLines: 1,
                  softWrap: false,
                ),
                const SizedBox(height: 6.0),
                Text(
                  item.description,
                  style: const TextStyle(fontSize: 13, color: Colors.grey),
                  overflow: TextOverflow.ellipsis,
                  maxLines: 5,
                  softWrap: true,
                ),
              ],
            ),
          ),
          if (item.onSetting != null) ...[
            const SizedBox(width: 8),
            IconButton(
              icon: const Icon(Symbols.settings, size: 22),
              onPressed: item.onSetting,
              tooltip: los.settings,
            ),
          ],
        ],
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
