import 'dart:developer';

import 'package:flutter/material.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/model/wave.dart';
import 'package:sen/screen/level_maker/waves/dino_page.dart';
import 'package:sen/screen/level_maker/waves/frost_wind_page.dart';
import 'package:sen/screen/level_maker/waves/jam_page.dart';
import 'package:sen/screen/level_maker/waves/low_tide_page.dart';
import 'package:sen/screen/level_maker/waves/parachute_page.dart';
import 'package:sen/screen/level_maker/waves/portal_page.dart';
import 'package:sen/screen/level_maker/waves/raiding_party_page.dart';
import 'package:sen/screen/level_maker/waves/regular_page.dart';
import 'package:sen/screen/level_maker/waves/spider_page.dart';
import 'package:sen/screen/level_maker/waves/storm_page.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/screen/level_maker/waves/tidal_page.dart';

class WaveManager extends StatefulWidget {
  const WaveManager({
    super.key,
    required this.waves,
    required this.zombies,
    required this.resource,
    required this.dinos,
    required this.levelModule,
  });

  final List<List<Wave>> waves;

  final List<String> zombies;

  final String resource;

  final List<String> dinos;

  final LevelModule levelModule;

  @override
  State<WaveManager> createState() => _WaveManagerState();
}

class _WaveManagerState extends State<WaveManager> {
  late ScrollController _scrollController;

  void _scrollDown() {
    _scrollController.animateTo(
      _scrollController.position.maxScrollExtent,
      duration: const Duration(milliseconds: 500),
      curve: Curves.fastOutSlowIn,
    );
  }

  @override
  void initState() {
    super.initState();
    _scrollController = ScrollController();
  }

  @override
  void dispose() {
    _scrollController.dispose();
    super.dispose();
  }

  void _waveNavigate(Wave wave, int index) async {
    final waveTypeToPageBuilder = {
      RegularWave: () => RegularPage(wave: wave as RegularWave, index: index),
      LowTide: () => LowTidePage(
            zombies: widget.zombies,
            wave: wave as LowTide,
            index: index,
          ),
      StormEvent: () => StormPage(
            resource: widget.resource,
            wave: wave as StormEvent,
            index: index,
            zombies: widget.zombies,
          ),
      ParachuteRain: () => ParachutePage(
            wave: wave as ParachuteRain,
            index: index,
            zombies: widget.zombies,
          ),
      SpiderRain: () => SpiderPage(
            wave: wave as SpiderRain,
            index: index,
            zombies: widget.zombies,
          ),
      RaidingParty: () => RaidingPartyPage(
            wave: wave as RaidingParty,
            index: index,
          ),
      FrostWind: () => FrostWindPage(
            wave: wave as FrostWind,
            index: index,
          ),
      DinoWave: () => DinoPage(
            wave: wave as DinoWave,
            index: index,
            dino: widget.dinos,
          ),
      TidalChange: () => TidalPage(
            wave: wave as TidalChange,
            index: index,
          ),
      JamWave: () => JamPage(
            wave: wave as JamWave,
            index: index,
            notificationEvent: widget.levelModule.notificationEvent!,
          ),
      PortalWave: () => PortalPage(
            wave: wave as PortalWave,
            index: index,
            portal: widget.levelModule.portal!,
          ),
    };

    final pageBuilder = waveTypeToPageBuilder[wave.runtimeType];

    if (pageBuilder != null) {
      await Navigator.of(context).push(
        MaterialPageRoute(
          builder: (context) => pageBuilder(),
        ),
      );
      if (mounted) {
        setState(() {});
      }
    } else {
      log('Unsupported wave type: ${wave.runtimeType}');
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Padding(
        padding: const EdgeInsets.all(8.0),
        child: SingleChildScrollView(
          controller: _scrollController,
          child: Column(
            children: widget.waves.asMap().entries.map(
              (e) {
                final index = e.key;
                final value = e.value;
                return _ExpandedWave(
                  wave: value,
                  index: index + 1,
                  onNavigate: _waveNavigate,
                  onDelete: () {
                    setState(() {
                      widget.waves.removeAt(index);
                    });
                  },
                );
              },
            ).toList(),
          ),
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: () {
          setState(() {
            widget.waves.add([RegularWave(zombies: [])]);
          });
          _scrollDown();
        },
        child: const Icon(Symbols.add),
      ),
    );
  }
}

class _ExpandedWave extends StatefulWidget {
  const _ExpandedWave({
    required this.wave,
    required this.index,
    required this.onNavigate,
    required this.onDelete,
  });

  final List<Wave> wave;
  final void Function(Wave wave, int index) onNavigate;
  final void Function() onDelete;
  final int index;

  @override
  State<_ExpandedWave> createState() => __ExpandedWaveState();
}

class __ExpandedWaveState extends State<_ExpandedWave> {
  bool _isExpanded = false;

  IconData _exchangeEvent(Wave wave) {
    final waveTypeToIcon = {
      RegularWave: Symbols.circle,
      StormEvent: Symbols.storm,
      LowTide: Symbols.waves,
      ParachuteRain: Symbols.paragliding,
      SpiderRain: Symbols.falling,
      RaidingParty: Symbols.rainy_heavy,
      FrostWind: Symbols.tornado,
      DinoWave: Symbols.pets,
      TidalChange: Symbols.surfing,
      JamWave: Symbols.celebration,
      PortalWave: Symbols.priority_high,
    };
    return waveTypeToIcon[wave.runtimeType] ?? Symbols.waves;
  }

  String _exchangeEventName(MapEntry<int, Wave> e, int index) {
    final value = e.value;
    final los = AppLocalizations.of(context)!;
    final waveTypeToLocalization = {
      RegularWave: los.regular_wave,
      LowTide: los.low_tide,
      StormEvent: los.storm_event,
      ParachuteRain: los.parachute_rain,
      SpiderRain: los.spider_rain,
      RaidingParty: los.raiding_party,
      FrostWind: los.frost_wind,
      DinoWave: los.dino_wave,
      TidalChange: los.tidal_change,
      JamWave: los.jam_wave,
      PortalWave: los.portal_wave,
    };
    final waveTypeName = waveTypeToLocalization[value.runtimeType] ?? '';
    return '${los.wave} $index: $waveTypeName';
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return SizedBox(
      width: double.infinity,
      child: Card(
        child: Column(
          children: [
            _buildHeader(context, los),
            if (_isExpanded) _buildWaveList(context, los),
          ],
        ),
      ),
    );
  }

  Widget _buildHeader(
    BuildContext context,
    AppLocalizations los,
  ) {
    final waveFactories = _waveFactories(los);
    return ListTile(
      leading: const Icon(Symbols.waves),
      title: Text('${los.wave} ${widget.index}'),
      trailing: Row(
        mainAxisSize: MainAxisSize.min,
        children: [
          _buildPopupMenuButton(waveFactories),
          const SizedBox(width: 15.0),
          _buildExpandIcon(los),
          const SizedBox(width: 15.0),
          _buildDeleteButton(los),
        ],
      ),
    );
  }

  Widget _buildPopupMenuButton(
    Map<String, Function()> waveFactories,
  ) {
    return PopupMenuButton<String>(
      onSelected: (String item) {
        final waveFactory = waveFactories[item];
        if (waveFactory != null) {
          setState(() {
            widget.wave.add(waveFactory());
          });
        }
      },
      itemBuilder: (BuildContext context) => waveFactories.entries
          .map(
            (entry) => PopupMenuItem<String>(
              value: entry.key,
              child: Text(entry.key),
            ),
          )
          .toList(),
    );
  }

  Widget _buildExpandIcon(
    AppLocalizations los,
  ) {
    return Tooltip(
      message: los.expand,
      child: ExpandIcon(
        onPressed: (value) {
          setState(() {
            _isExpanded = !value;
          });
        },
        isExpanded: _isExpanded,
      ),
    );
  }

  Widget _buildDeleteButton(
    AppLocalizations los,
  ) {
    return Tooltip(
      message: los.delete,
      child: IconButton(
        onPressed: widget.onDelete,
        icon: const Icon(Symbols.delete),
      ),
    );
  }

  Widget _buildWaveList(
    BuildContext context,
    AppLocalizations los,
  ) {
    return Column(
      children: widget.wave.asMap().entries.map(
        (e) {
          final index = e.key;
          final element = e.value;
          return _buildWaveItem(e, index, element, los);
        },
      ).toList(),
    );
  }

  Widget _buildWaveItem(
    MapEntry<int, Wave> e,
    int index,
    Wave element,
    AppLocalizations los,
  ) {
    return Container(
      margin: const EdgeInsets.symmetric(horizontal: 16.0, vertical: 4.0),
      child: ListTile(
        leading: Icon(_exchangeEvent(element), color: Colors.blueAccent),
        title: Text(_exchangeEventName(e, widget.index)),
        trailing: Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            _buildEditButton(element, los),
            const SizedBox(width: 8.0),
            _buildDeleteWaveButton(index, los),
          ],
        ),
      ),
    );
  }

  Widget _buildEditButton(
    Wave element,
    AppLocalizations los,
  ) {
    return Tooltip(
      message: los.edit,
      child: IconButton(
        onPressed: () => widget.onNavigate(element, widget.index),
        icon: const Icon(Symbols.edit),
      ),
    );
  }

  Widget _buildDeleteWaveButton(
    int index,
    AppLocalizations los,
  ) {
    return Tooltip(
      message: los.delete,
      child: IconButton(
        onPressed: () {
          setState(() {
            widget.wave.removeAt(index);
          });
        },
        icon: const Icon(Symbols.delete),
      ),
    );
  }

  Map<String, Function()> _waveFactories(
    AppLocalizations los,
  ) {
    return {
      los.regular_wave: () => RegularWave(zombies: []),
      los.low_tide: () => LowTide.withDefault(),
      los.storm_event: () => StormEvent.withDefault(),
      los.parachute_rain: () => ParachuteRain.withDefault(),
      los.spider_rain: () => SpiderRain.withDefault(),
      los.raiding_party: () => RaidingParty.withDefault(),
      los.frost_wind: () => FrostWind.withDefault(),
      los.dino_wave: () => DinoWave.withDefault(),
      los.tidal_change: () => TidalChange.withDefault(),
      los.jam_wave: () => JamWave.withDefault(),
      los.portal_wave: () => PortalWave.withDefault(),
    };
  }
}
