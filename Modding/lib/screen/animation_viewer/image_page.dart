import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/bloc/selected_image_bloc/selected_image_bloc.dart';

class ImagePage extends StatelessWidget {
  const ImagePage({
    super.key,
    required this.image,
  });

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
          leading: const Icon(Symbols.image, size: 40),
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
