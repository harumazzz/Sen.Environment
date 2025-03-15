import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../bloc/selected_image_bloc/selected_image_bloc.dart';
import '../../extension/platform.dart';
import 'visual_helper.dart';
import '../../service/ui_helper.dart';

class ImagePage extends StatelessWidget {
  const ImagePage({super.key, required this.visualHelper, required this.image});

  final VisualHelper visualHelper;

  final List<String> image;

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);

    return ListView.builder(
      padding: const EdgeInsets.symmetric(horizontal: 6.0, vertical: 4.0),
      itemCount: image.length,
      itemBuilder:
          (context, index) => Card(
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
                  visualHelper.imageSource[index] == null
                      ? null
                      : () async => await UIHelper.showFullImage(
                        context: context,
                        name: image[index],
                        image: visualHelper.imageSource[index]!,
                      ),
              leading:
                  visualHelper.imageSource[index] == null
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
                image[index],
                style: theme.textTheme.titleSmall?.copyWith(
                  color: theme.colorScheme.onSurface,
                ),
                overflow: TextOverflow.ellipsis,
                maxLines: CurrentPlatform.isMobile ? 4 : null,
              ),
              trailing: Checkbox.adaptive(
                value: context.watch<SelectedImageBloc>().state.value[index],
                onChanged: (_) {
                  context.read<SelectedImageBloc>().add(
                    SelectedImageToggleEvent(index: index),
                  );
                },
                side: BorderSide(color: theme.colorScheme.outline, width: 1.5),
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
          ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<VisualHelper>('visualHelper', visualHelper),
    );
    properties.add(IterableProperty<String>('image', image));
  }
}
