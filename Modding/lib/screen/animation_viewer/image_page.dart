import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/screen/animation_viewer/provider/selected_image.dart';

class ImagePage extends ConsumerWidget {
  const ImagePage({
    super.key,
    required this.image,
  });

  final List<String> image;

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return ListView.builder(
      itemCount: image.length,
      itemBuilder: (context, index) => Card(
        margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
        elevation: 4,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(12),
        ),
        child: ListTile(
          contentPadding: const EdgeInsets.all(12),
          leading: const Icon(Symbols.image, size: 40),
          title: Text(image[index], style: Theme.of(context).textTheme.titleSmall),
          trailing: Checkbox(
            value: ref.watch(selectedImageListProvider)[index],
            onChanged: (_) {
              ref.read(selectedImageListProvider.notifier).toggle(index);
            },
          ),
        ),
      ),
    );
  }
}
