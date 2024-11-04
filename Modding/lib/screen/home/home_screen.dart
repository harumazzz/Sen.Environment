import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/model/item.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/animation_viewer/main_screen.dart';
import 'package:sen/screen/javascript_category/js_pick.dart';
import 'package:sen/screen/level_maker/level_maker.dart';
import 'package:sen/screen/map_editor/map_editor.dart';
import 'package:sen/screen/shell/shell_screen.dart';
import 'package:page_transition/page_transition.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/widget/animated_floating.dart';

class HomeScreen extends ConsumerStatefulWidget {
  const HomeScreen({super.key});

  @override
  ConsumerState<HomeScreen> createState() => _HomeScreenState();
}

class _HomeScreenState extends ConsumerState<HomeScreen> {
  late List<Item> items;

  @override
  void initState() {
    super.initState();
  }

  void _initWidget() {
    _initShellWidget();
    _initJSModule();
    _initAnimationViewer();
    _initLevelMaker();
    _initMapEditor();
  }

  void _initAnimationViewer() {
    items[2].onWidget = () {
      return const AnimationViewer();
    };
  }

  void _initShellWidget() {
    items[0].onWidget = () {
      return const ShellScreen(arguments: []);
    };
  }

  void _initJSModule() {
    final setting = ref.read(settingProvider);
    items[1].onWidget = () {
      return JsPick(
        holder: setting.toolChain,
      );
    };
  }

  void _initLevelMaker() {
    items[3].onWidget = () {
      return const LevelMaker();
    };
  }

  void _initMapEditor() {
    items[4].onWidget = () {
      return const MapEditor();
    };
  }

  Widget _buildUI() {
    final los = AppLocalizations.of(context)!;
    final setting = ref.watch(settingProvider);
    if (Platform.isMacOS || Platform.isLinux || Platform.isWindows) {
      final screenWidth = MediaQuery.of(context).size.width;
      const itemWidth = 250.0;
      final crossAxisCount = (screenWidth / itemWidth).floor();
      return GridView.builder(
        gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: crossAxisCount,
          childAspectRatio: 1.0,
        ),
        itemCount: items.length,
        itemBuilder: (context, index) {
          final item = items[index];
          return Tooltip(
            message: item.title,
            child: Card(
              clipBehavior: Clip.none,
              child: InkWell(
                splashColor: Colors.blue.withAlpha(30),
                onTap: setting.isValid
                    ? () {
                        Navigator.push(
                          context,
                          PageTransition(
                            duration: const Duration(milliseconds: 300),
                            type: PageTransitionType.rightToLeft,
                            child: item.onWidget(),
                          ),
                        );
                      }
                    : null,
                child: Container(
                  margin: const EdgeInsets.symmetric(horizontal: 8.0),
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      AnimatedFloating(child: item.icon),
                      const SizedBox(height: 8),
                      Text(
                        item.title,
                        textAlign: TextAlign.center,
                        maxLines: 4,
                        style: const TextStyle(fontWeight: FontWeight.bold),
                      ),
                      const SizedBox(height: 4),
                      Text(
                        item.description,
                        textAlign: TextAlign.center,
                        maxLines: 4,
                        overflow: TextOverflow.ellipsis,
                      ),
                      const SizedBox(height: 15),
                      setting.isValid
                          ? Container()
                          : Text(los.toolchain_is_invalid),
                    ],
                  ),
                ),
              ),
            ),
          );
        },
      );
    } else {
      return ListView.builder(
        itemCount: items.length,
        itemBuilder: (context, index) {
          final item = items[index];
          return Padding(
            padding: const EdgeInsets.symmetric(vertical: 4.0),
            child: Card(
              clipBehavior: Clip.hardEdge,
              child: InkWell(
                splashColor: Colors.blue.withAlpha(30),
                onTap: setting.isValid
                    ? () {
                        Navigator.push(
                          context,
                          PageTransition(
                            duration: const Duration(milliseconds: 300),
                            type: PageTransitionType.rightToLeft,
                            child: item.onWidget(),
                          ),
                        );
                      }
                    : null,
                child: ListTile(
                  leading: item.icon,
                  title: Text(
                    item.title,
                    maxLines: 4,
                  ),
                  subtitle: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      const SizedBox(height: 2),
                      Text(
                        item.description,
                        maxLines: 2,
                        overflow: TextOverflow.ellipsis,
                      ),
                      const SizedBox(height: 8),
                      setting.isValid
                          ? Container()
                          : Text(los.toolchain_is_invalid),
                    ],
                  ),
                  onTap: setting.isValid
                      ? () {
                          Navigator.push(
                            context,
                            PageTransition(
                              duration: const Duration(milliseconds: 300),
                              type: PageTransitionType.rightToLeft,
                              child: item.onWidget(),
                            ),
                          );
                        }
                      : null,
                ),
              ),
            ),
          );
        },
      );
    }
  }

  void _initItem() {
    final los = AppLocalizations.of(context)!;
    items = [
      Item(
        title: los.shell,
        description: los.shell_description,
        icon: Icon(
          Symbols.terminal_rounded,
          size: 50,
          color: Colors.blueGrey.shade600,
        ),
      ),
      Item(
        title: los.js_execute,
        description: los.js_execute_description,
        icon: Icon(
          Symbols.javascript_rounded,
          size: 50,
          color: Colors.yellow.shade600,
        ),
      ),
      Item(
        title: los.animation_viewer,
        description: los.animation_viewer_description,
        icon: Icon(
          Symbols.animated_images,
          size: 50,
          color: Colors.green.shade700,
        ),
      ),
      Item(
        title: los.level_maker,
        description: los.level_maker_description,
        icon: Icon(
          Symbols.build,
          size: 50,
          color: Colors.cyan.shade600,
        ),
      ),
      Item(
        title: los.map_editor,
        description: los.map_editor_description,
        icon: Icon(
          Symbols.map,
          size: 50,
          color: Colors.lightBlue.shade600,
        ),
      ),
    ];
  }

  @override
  Widget build(BuildContext context) {
    _initItem();
    _initWidget();
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: _buildUI(),
    );
  }
}
