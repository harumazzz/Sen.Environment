import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/bloc/selected_sprite_bloc/selected_sprite_bloc.dart';
import 'package:sen/screen/animation_viewer/visual_helper.dart';

class SpritePage extends StatelessWidget {
  const SpritePage({
    super.key,
    required this.staticController,
    required this.visualHelper,
    required this.sprite,
  });

  final AnimationController staticController;
  final VisualHelper visualHelper;
  final List<String> sprite;

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);

    return ListView.builder(
      padding: const EdgeInsets.symmetric(horizontal: 6.0, vertical: 4.0),
      itemCount: sprite.length,
      itemBuilder: (context, index) => Card(
        elevation: 4.0,
        shadowColor: theme.colorScheme.shadow.withValues(alpha: 0.2),
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(32.0),
        ),
        child: ListTile(
          contentPadding: const EdgeInsets.symmetric(horizontal: 16.0, vertical: 4.0),
          leading: !visualHelper.containSprite(index)
              ? Icon(
                  Symbols.broken_image,
                  color: theme.colorScheme.onSurfaceVariant,
                  size: 32.0,
                )
              : ClipRRect(
                  borderRadius: BorderRadius.circular(20.0),
                  child: FittedBox(
                    fit: BoxFit.contain,
                    child: UnconstrainedBox(
                      child: visualHelper.visualizeSprite(index, staticController),
                    ),
                  ),
                ),
          title: Text(
            sprite[index],
            style: theme.textTheme.titleSmall?.copyWith(
              color: theme.colorScheme.onSurface,
            ),
            overflow: TextOverflow.ellipsis,
          ),
          trailing: Checkbox.adaptive(
            value: context.watch<SelectedSpriteBloc>().state.value[index],
            onChanged: (_) {
              context.read<SelectedSpriteBloc>().add(SelectedSpriteToggleEvent(index: index));
            },
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(8),
            ),
            activeColor: theme.colorScheme.primary,
            checkColor: theme.colorScheme.onPrimary,
          ),
          tileColor: theme.colorScheme.surfaceContainerHighest,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(16),
          ),
        ),
      ),
    );
  }
}
