import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/screen/animation_viewer/visual_helper.dart';

class MediaPage extends StatelessWidget {
  const MediaPage({
    super.key,
    required this.media,
  });

  final List<String> media;

  @override
  Widget build(BuildContext context) {
    return ListView.builder(
      itemCount: media.length,
      itemBuilder: (context, index) => Card(
        margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
        elevation: 4,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(12),
        ),
        child: ListTile(
          contentPadding: const EdgeInsets.all(12),
          leading: VisualHelper.imageSource[index] == null
              ? const Icon(Symbols.broken_image, size: 40)
              : ClipRRect(
                  borderRadius: BorderRadius.circular(8),
                  child: Image(
                    image: VisualHelper.imageSource[index]!,
                    width: 40,
                    height: 40,
                    fit: BoxFit.cover,
                  ),
                ),
          title: Text(media[index], style: Theme.of(context).textTheme.titleSmall),
        ),
      ),
    );
  }
}
