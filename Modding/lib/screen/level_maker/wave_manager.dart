import 'dart:developer';

import 'package:flutter/material.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/model/wave.dart';
import 'package:sen/screen/level_maker/waves/low_tide_page.dart';
import 'package:sen/screen/level_maker/waves/parachute_page.dart';
import 'package:sen/screen/level_maker/waves/regular_page.dart';
import 'package:sen/screen/level_maker/waves/storm_page.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class WaveManager extends StatefulWidget {
  const WaveManager({
    super.key,
    required this.waves,
    required this.zombies,
    required this.resource,
  });

  final List<List<Wave>> waves;

  final List<String> zombies;

  final String resource;

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
    };

    final pageBuilder = waveTypeToPageBuilder[wave.runtimeType];

    if (pageBuilder != null) {
      await Navigator.of(context).push(
        MaterialPageRoute(
          builder: (context) => pageBuilder(),
        ),
      );
      setState(() {});
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
  IconData _exchangeEvent(Wave wave) {
    final waveTypeToIcon = {
      RegularWave: Symbols.circle,
      StormEvent: Symbols.storm,
      LowTide: Symbols.waves,
      ParachuteRain: Symbols.paragliding,
    };
    return waveTypeToIcon[wave.runtimeType] ?? Symbols.waves;
  }

  bool _isExpanded = false;

  String _exchangeEventName(MapEntry<int, Wave> e, int index) {
    final value = e.value;
    final los = AppLocalizations.of(context)!;
    final waveTypeToLocalization = {
      RegularWave: los.regular_wave,
      LowTide: los.low_tide,
      StormEvent: los.storm_event,
      ParachuteRain: los.parachute_rain,
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
            ListTile(
              leading: const Icon(Symbols.waves),
              title: Text('${los.wave} ${widget.index}'),
              trailing: Row(
                mainAxisSize: MainAxisSize.min,
                children: [
                  PopupMenuButton<String>(
                    onSelected: (String item) {},
                    itemBuilder: (BuildContext context) =>
                        <PopupMenuEntry<String>>[
                      PopupMenuItem<String>(
                        child: Text(los.regular_wave),
                        onTap: () {
                          setState(() {
                            widget.wave.add(RegularWave(zombies: []));
                          });
                        },
                      ),
                      PopupMenuItem<String>(
                        child: Text(los.low_tide),
                        onTap: () {
                          setState(() {
                            widget.wave.add(LowTide.withDefault());
                          });
                        },
                      ),
                      PopupMenuItem<String>(
                        child: Text(los.sandstorm),
                        onTap: () {
                          setState(() {
                            widget.wave.add(StormEvent.withDefault());
                          });
                        },
                      ),
                      PopupMenuItem<String>(
                        child: Text(los.parachute_rain),
                        onTap: () {
                          setState(() {
                            widget.wave.add(ParachuteRain.withDefault());
                          });
                        },
                      ),
                    ],
                  ),
                  const SizedBox(width: 15.0),
                  Tooltip(
                    message: los.expand,
                    child: ExpandIcon(
                      onPressed: (value) {
                        setState(() {
                          _isExpanded = !value;
                        });
                      },
                      isExpanded: _isExpanded,
                    ),
                  ),
                  const SizedBox(width: 15.0),
                  Tooltip(
                    message: los.delete,
                    child: IconButton(
                      onPressed: widget.onDelete,
                      icon: const Icon(Symbols.delete),
                    ),
                  ),
                ],
              ),
            ),
            if (_isExpanded)
              ...widget.wave.asMap().entries.map(
                (e) {
                  final index = e.key;
                  final element = e.value;
                  return Container(
                    margin: const EdgeInsets.symmetric(
                      horizontal: 16.0,
                      vertical: 4.0,
                    ),
                    child: ListTile(
                      leading: Icon(_exchangeEvent(element),
                          color: Colors.blueAccent),
                      title: Text(_exchangeEventName(e, widget.index)),
                      trailing: Row(
                        mainAxisSize: MainAxisSize.min,
                        children: [
                          Tooltip(
                            message: los.edit,
                            child: IconButton(
                              onPressed: () =>
                                  widget.onNavigate(element, widget.index),
                              icon: const Icon(Symbols.edit),
                            ),
                          ),
                          const SizedBox(width: 8.0),
                          Tooltip(
                            message: los.delete,
                            child: IconButton(
                              onPressed: () {
                                setState(() {
                                  widget.wave.removeAt(index);
                                });
                              },
                              icon: const Icon(Symbols.delete),
                            ),
                          ),
                        ],
                      ),
                    ),
                  );
                },
              ),
          ],
        ),
      ),
    );
  }
}
