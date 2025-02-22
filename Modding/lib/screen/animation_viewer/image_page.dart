import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/bloc/selected_image_bloc/selected_image_bloc.dart';
import 'package:sen/screen/animation_viewer/visual_helper.dart';

class ImagePage extends StatelessWidget {
  const ImagePage({
    super.key,
    required this.visualHelper,
    required this.image,
  });

  final VisualHelper visualHelper;

  final List<String> image;

  @override
  Widget build(BuildContext context) {
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
          leading: visualHelper.imageSource[index] == null
              ? const Icon(Symbols.broken_image, size: 40)
              : ClipRRect(
                  borderRadius: BorderRadius.circular(8),
                  child: Image(
                    image: visualHelper.imageSource[index]!,
                    width: 40,
                    height: 40,
                    fit: BoxFit.cover,
                  ),
                ),
          title: Text(image[index], style: Theme.of(context).textTheme.titleSmall),
          trailing: Checkbox(
            value: context.watch<SelectedImageBloc>().state.value[index],
            onChanged: (_) {
              context.read<SelectedImageBloc>().add(SelectedImageToggleEvent(index: index));
            },
          ),
        ),
      ),
    );
  }
}
