import 'dart:io';

import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/screen/animation_viewer/image_page.dart';
import 'package:sen/screen/animation_viewer/media_page.dart';
import 'package:sen/screen/animation_viewer/sprite_page.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/screen/animation_viewer/visual_helper.dart';

class MediaScreen extends StatelessWidget {
  final List<String> sprite;
  final List<String> image;
  final List<String> media;
  final VisualHelper visualHelper;
  final AnimationController staticController;

  const MediaScreen({
    super.key,
    required this.sprite,
    required this.image,
    required this.media,
    required this.visualHelper,
    required this.staticController,
  });

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return DefaultTabController(
      initialIndex: 0,
      length: 3,
      child: Scaffold(
        appBar: AppBar(
          forceMaterialTransparency: Platform.isWindows || Platform.isLinux || Platform.isMacOS,
          automaticallyImplyLeading: false,
          centerTitle: true,
          backgroundColor: Colors.transparent,
          bottom: PreferredSize(
            preferredSize: const Size.fromHeight(48.0),
            child: Container(
              decoration: BoxDecoration(
                border: Border(
                  bottom: BorderSide(
                    color: Theme.of(context).dividerColor,
                    width: 2.0,
                  ),
                ),
              ),
              child: TabBar(
                indicatorColor: Theme.of(context).colorScheme.secondary,
                tabs: <Widget>[
                  Tab(
                    icon: const Icon(Symbols.folder),
                    text: los.media,
                  ),
                  Tab(
                    icon: const Icon(Symbols.folder),
                    text: los.image,
                  ),
                  Tab(
                    icon: const Icon(Symbols.folder),
                    text: los.sprite,
                  ),
                ],
              ),
            ),
          ),
        ),
        body: Padding(
          padding: const EdgeInsets.all(8.0),
          child: TabBarView(
            children: <Widget>[
              MediaPage(
                visualHelper: visualHelper,
                media: media,
              ),
              ImagePage(
                visualHelper: visualHelper,
                image: image,
              ),
              SpritePage(
                staticController: staticController,
                visualHelper: visualHelper,
                sprite: sprite,
              ),
            ],
          ),
        ),
      ),
    );
  }
}
