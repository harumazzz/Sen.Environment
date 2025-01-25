import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/model/wave.dart';
import 'package:sen/screen/level_maker/code_preview.dart';
import 'package:sen/screen/level_maker/level_definition.dart';
import 'package:sen/screen/level_maker/level_initializer.dart';
import 'package:sen/screen/level_maker/music_type.dart';
import 'package:sen/screen/level_maker/wave_manager.dart';
import 'package:sen/service/file_helper.dart';
import 'package:sen/widget/hotkey.dart';

class LevelMaker extends StatefulWidget {
  const LevelMaker({super.key});

  @override
  State<LevelMaker> createState() => _LevelMakerState();
}

class _LevelMakerState extends State<LevelMaker> with TickerProviderStateMixin {
  late TabController _tabController;

  late TextEditingController _levelNameController;
  late TextEditingController _levelDescriptionController;
  late TextEditingController _levelStageController;
  late TextEditingController _lawnMowerController;
  late bool _hasSunFalling;
  late List<List<Wave>> _waves;

  late MusicType _musicType;

  late String _resource;
  List<String> _plants = [], _zombies = [], _gridItems = [], _dinos = [];

  late LevelModule _levelModule;

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
    _loadResources();
  }

  void _loadResources() async {
    final los = AppLocalizations.of(context)!;
    final setting = BlocProvider.of<SettingsCubit>(context).state.levelMakerResource;
    if (setting.isEmpty) {
      throw Exception(los.please_configure_resource_location);
    }
    _resource = setting;
    _plants = await FileHelper.readDirectoryAsync(
      source: '$_resource/plant',
      recursive: false,
    );
    _zombies = await FileHelper.readDirectoryAsync(
      source: '$_resource/zombie',
      recursive: false,
    );
    _gridItems = await FileHelper.readDirectoryAsync(
      source: '$_resource/item',
      recursive: false,
    );
    _dinos = await FileHelper.readDirectoryAsync(
      source: '$_resource/dino',
      recursive: false,
    );
    _levelModule = LevelModule.fromJson(
      FileHelper.readJson(source: '$_resource/level_module.json'),
    );
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

    return HotkeyBuilder(
      child: Scaffold(
        appBar: AppBar(
          title: Text(los.level_maker),
          bottom: TabBar(
            controller: _tabController,
            tabs: const <Widget>[
              Tab(icon: Icon(Symbols.settings)),
              Tab(icon: Icon(Symbols.grid_3x3)),
              Tab(icon: Icon(Symbols.waves)),
              Tab(icon: Icon(Symbols.code_blocks)),
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
              levelModule: _levelModule,
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
              resource: _resource,
              waves: _waves,
              zombies: _zombies,
              dinos: _dinos,
              levelModule: _levelModule,
            ),
            CodePreview(
              levelNameController: _levelNameController,
              levelDescriptionController: _levelDescriptionController,
              levelStageController: _levelStageController,
              lawnMowerController: _lawnMowerController,
              hasSunFalling: _hasSunFalling,
              musicType: _musicType,
              wave: _waves,
            ),
          ],
        ),
      ),
    );
  }
}
