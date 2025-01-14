import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/bloc/selected_sprite_bloc/selected_sprite_bloc.dart';

class SpritePage extends StatelessWidget {
  const SpritePage({
    super.key,
    required this.sprite,
  });

  final List<String> sprite;

  @override
  Widget build(BuildContext context) {
    return ListView.builder(
      itemCount: sprite.length,
      itemBuilder: (context, index) => Card(
        margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
        elevation: 4,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(12),
        ),
        child: ListTile(
          contentPadding: const EdgeInsets.all(12),
          leading: const Icon(Symbols.image, size: 40),
          title: Text(sprite[index], style: Theme.of(context).textTheme.titleSmall),
          trailing: Checkbox(
            value: context.watch<SelectedSpriteBloc>().state.value[index],
            onChanged: (_) {
              context.read<SelectedSpriteBloc>().add(SelectedSpriteToggleEvent(index: index));
            },
          ),
        ),
      ),
    );
  }
}
