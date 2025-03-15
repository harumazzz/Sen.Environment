import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../bloc/selected_sprite_bloc/selected_sprite_bloc.dart';
import '../../extension/platform.dart';
import 'visual_helper.dart';
import '../../service/ui_helper.dart';

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
      itemBuilder: (context, index) {
        final containSprite = visualHelper.containSprite(index);
        return Card(
          elevation: 4.0,
          shadowColor: theme.colorScheme.shadow.withValues(alpha: 0.2),
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(32.0),
          ),
          child: ListTile(
            contentPadding: const EdgeInsets.symmetric(
              horizontal: 16.0,
              vertical: 4.0,
            ),
            onTap:
                !containSprite
                    ? null
                    : () async {
                      await UIHelper.showWidget(
                        context: context,
                        name: sprite[index],
                        child: visualHelper.visualizeSprite(
                          index,
                          staticController,
                        ),
                      );
                    },
            leading:
                !containSprite
                    ? Icon(
                      Symbols.broken_image,
                      color: theme.colorScheme.onSurfaceVariant,
                      size: 32.0,
                    )
                    : ClipRRect(
                      borderRadius: BorderRadius.circular(20.0),
                      child: FittedBox(
                        child: UnconstrainedBox(
                          child: visualHelper.visualizeSprite(
                            index,
                            staticController,
                          ),
                        ),
                      ),
                    ),
            title: Text(
              sprite[index],
              style: theme.textTheme.titleSmall?.copyWith(
                color: theme.colorScheme.onSurface,
              ),
              overflow: TextOverflow.ellipsis,
              maxLines: CurrentPlatform.isMobile ? 4 : null,
            ),
            trailing: Checkbox.adaptive(
              value: context.watch<SelectedSpriteBloc>().state.value[index],
              side: BorderSide(color: theme.colorScheme.outline, width: 1.5),
              onChanged: (_) {
                context.read<SelectedSpriteBloc>().add(
                  SelectedSpriteToggleEvent(index: index),
                );
              },
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(8),
              ),
              activeColor: theme.colorScheme.primary,
              checkColor: theme.colorScheme.onPrimary,
            ),
            tileColor: UIHelper.colorOf(context),
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(16),
            ),
          ),
        );
      },
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<AnimationController>(
        'staticController',
        staticController,
      ),
    );
    properties.add(
      DiagnosticsProperty<VisualHelper>('visualHelper', visualHelper),
    );
    properties.add(IterableProperty<String>('sprite', sprite));
  }
}
