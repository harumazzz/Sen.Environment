import 'package:flutter/material.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/model/wave.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/level_maker/code_preview.dart';
import 'package:sen/screen/level_maker/level_definition.dart';
import 'package:sen/screen/level_maker/level_initializer.dart';
import 'package:sen/screen/level_maker/music_type.dart';
import 'package:sen/screen/level_maker/wave_manager.dart';
import 'package:sen/service/file_service.dart';

class LevelMaker extends ConsumerStatefulWidget {
  const LevelMaker({super.key});

  @override
  ConsumerState<LevelMaker> createState() => _LevelMakerState();
}

class _LevelMakerState extends ConsumerState<LevelMaker>
    with TickerProviderStateMixin {
  late TabController _tabController;

  late TextEditingController _levelNameController;
  late TextEditingController _levelDescriptionController;
  late TextEditingController _levelStageController;
  late TextEditingController _lawnMowerController;
  late bool _hasSunFalling;
  late List<List<Wave>> _waves;

  late MusicType _musicType;

  List<String> _plants = [], _zombies = [], _gridItems = [];

  @override
  void initState() {
    super.initState();
    _tabController = TabController(length: 4, vsync: this);
    _levelNameController = TextEditingController();
    _levelDescriptionController = TextEditingController();
    _levelStageController = TextEditingController();
    _lawnMowerController = TextEditingController();
    _musicType = MusicType.normal;
    _waves = [];
    _hasSunFalling = true;
    WidgetsBinding.instance.addPostFrameCallback((_) {
      final setting = ref.read(settingProvider).toolChain;
      final resource = '$setting/resource/level_maker';
      setState(() {
        _plants = FileService.readDirectory(
          source: '$resource/plant',
          recursive: false,
        );
        _zombies = FileService.readDirectory(
          source: '$resource/zombie',
          recursive: false,
        );
        _gridItems = FileService.readDirectory(
          source: '$resource/item',
          recursive: false,
        );
      });
    });
  }

  @override
  void dispose() {
    _tabController.dispose();
    _levelNameController.dispose();
    _levelDescriptionController.dispose();
    _levelStageController.dispose();
    _lawnMowerController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Scaffold(
      appBar: AppBar(
        title: Text(los.level_maker),
        bottom: TabBar(
          controller: _tabController,
          tabs: const <Widget>[
            Tab(
              icon: Icon(Symbols.settings),
            ),
            Tab(
              icon: Icon(Symbols.grid_3x3),
            ),
            Tab(
              icon: Icon(Symbols.waves),
            ),
            Tab(
              icon: Icon(Symbols.code_blocks),
            ),
          ],
        ),
      ),
      body: TabBarView(
        controller: _tabController,
        children: <Widget>[
          LevelDefinition(
            levelNameController: _levelNameController,
            levelDescriptionController: _levelDescriptionController,
            lawnMowerController: _lawnMowerController,
            levelStageController: _levelStageController,
            musicType: _musicType,
            onChangeMusicType: (newValue) {
              if (newValue == null) return;
              setState(() {
                _musicType = newValue;
              });
            },
            hasSunFalling: _hasSunFalling,
            onToggleSunFalling: (bool? newValue) {
              if (newValue == null) return;
              setState(() {
                _hasSunFalling = newValue;
              });
            },
          ),
          LevelInitializer(
            plants: _plants,
            zombies: _zombies,
            gridItem: _gridItems,
          ),
          WaveManager(
            waves: _waves,
            zombies: _zombies,
          ),
          CodePreview(
            levelNameController: _levelNameController,
            levelDescriptionController: _levelDescriptionController,
            levelStageController: _levelStageController,
            lawnMowerController: _lawnMowerController,
            hasSunFalling: _hasSunFalling,
            musicType: _musicType,
          ),
        ],
      ),
    );
  }
}
