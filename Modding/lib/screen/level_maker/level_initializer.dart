import 'dart:developer';

import 'package:flutter/material.dart';
import 'package:sen/screen/level_maker/level_lawn.dart';
import 'package:sen/screen/level_maker/level_options.dart';

class LevelInitializer extends StatefulWidget {
  const LevelInitializer({
    super.key,
    required this.plants,
    required this.zombies,
    required this.gridItem,
  });

  final List<String> plants;
  final List<String> zombies;
  final List<String> gridItem;

  @override
  State<LevelInitializer> createState() => _LevelInitializerState();
}

class _LevelInitializerState extends State<LevelInitializer> {
  MemoryImage? _selectedItem;

  void _onItemSelected(MemoryImage item) {
    setState(() {
      _selectedItem = item;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: Column(
        children: [
          LevelLawn(
            selectedItem: _selectedItem,
            onTap: (row, col) {
              log('tapped $row and $col');
            },
          ),
          Expanded(
            child: LevelOptions(
              plants: widget.plants,
              zombies: widget.zombies,
              gridItem: widget.gridItem,
              onItemSelected: _onItemSelected,
            ),
          ),
        ],
      ),
    );
  }
}
