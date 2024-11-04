import 'package:flutter/material.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/model/wave.dart';
import 'package:sen/screen/level_maker/waves/regular_page.dart';

class WaveManager extends StatefulWidget {
  const WaveManager({
    super.key,
    required this.waves,
  });

  final List<List<Wave>> waves;

  @override
  State<WaveManager> createState() => _WaveManagerState();
}

class _WaveManagerState extends State<WaveManager> {
  late ScrollController _scrollController;

  void _scrollDown() {
    _scrollController.animateTo(
      _scrollController.position.maxScrollExtent,
      duration: Duration(milliseconds: 500),
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

  void _waveNavigate(
    Wave wave,
    int index,
  ) async {
    var state = null as Widget?;
    if (wave is RegularWave) {
      state = RegularPage(
        wave: wave,
        index: index,
      );
    }
    await Navigator.of(context).push(
      MaterialPageRoute(
        builder: (context) => state!,
      ),
    );
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    int index = 1;
    return Scaffold(
      body: Padding(
        padding: const EdgeInsets.all(8.0),
        child: SingleChildScrollView(
          controller: _scrollController,
          child: Column(
            children: widget.waves.map((e) {
              return _ExpandedWave(
                wave: e,
                index: index++,
                onNavigate: _waveNavigate,
              );
            }).toList(),
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
        child: Icon(Symbols.add),
      ),
    );
  }
}

class _ExpandedWave extends StatefulWidget {
  const _ExpandedWave({
    required this.wave,
    required this.index,
    required this.onNavigate,
  });

  final List<Wave> wave;

  final void Function(Wave wave, int index) onNavigate;

  final int index;

  @override
  State<_ExpandedWave> createState() => __ExpandedWaveState();
}

class __ExpandedWaveState extends State<_ExpandedWave> {
  IconData _exchangeEvent(Wave wave) {
    if (wave is RegularWave) {
      return Symbols.circle;
    }
    return Symbols.waves;
  }

  bool _isExpanded = false;

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      width: double.infinity,
      child: Card(
        child: Column(
          children: [
            ListTile(
              leading: Icon(Symbols.waves),
              title: Text('Wave ${widget.index}'),
              trailing: Tooltip(
                message: 'Expand',
                child: ExpandIcon(
                  onPressed: (value) {
                    setState(() {
                      _isExpanded = !value;
                    });
                  },
                  isExpanded: _isExpanded,
                ),
              ),
            ),
            if (_isExpanded)
              ...widget.wave.map(
                (e) => Container(
                  margin: EdgeInsets.symmetric(horizontal: 16.0, vertical: 4.0),
                  child: ListTile(
                    leading: Icon(_exchangeEvent(e), color: Colors.blueAccent),
                    title: Text(e.toString()),
                    trailing: IconButton(
                      onPressed: () => widget.onNavigate(e, widget.index),
                      icon: Icon(Symbols.play_arrow),
                    ),
                  ),
                ),
              ),
          ],
        ),
      ),
    );
  }
}
