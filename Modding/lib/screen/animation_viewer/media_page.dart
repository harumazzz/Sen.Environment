import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/screen/animation_viewer/visual_helper.dart';
import 'package:sen/service/ui_helper.dart';

class MediaPage extends StatelessWidget {
  const MediaPage({
    super.key,
    required this.media,
    required this.visualHelper,
  });

  final List<String> media;
  final VisualHelper visualHelper;

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);

    return ListView.builder(
      padding: const EdgeInsets.symmetric(horizontal: 6.0, vertical: 4.0),
      itemCount: media.length,
      itemBuilder: (context, index) => Card(
        elevation: 4.0,
        shadowColor: theme.colorScheme.shadow.withValues(alpha: 0.2),
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(32.0),
        ),
        child: ListTile(
          contentPadding: const EdgeInsets.symmetric(horizontal: 16.0, vertical: 4.0),
          onTap: visualHelper.imageSource[index] == null
              ? null
              : () async => await UIHelper.showFullImage(context, visualHelper.imageSource[index]!),
          leading: visualHelper.imageSource[index] == null
              ? Icon(
                  Symbols.broken_image,
                  color: theme.colorScheme.onSurfaceVariant,
                  size: 32.0,
                )
              : ClipRRect(
                  borderRadius: BorderRadius.circular(12),
                  child: Image(
                    image: visualHelper.imageSource[index]!,
                    width: 32.0,
                    height: 32.0,
                    fit: BoxFit.cover,
                  ),
                ),
          title: Text(
            media[index],
            style: theme.textTheme.labelLarge?.copyWith(
              color: theme.colorScheme.onSurface,
            ),
            overflow: TextOverflow.ellipsis,
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
