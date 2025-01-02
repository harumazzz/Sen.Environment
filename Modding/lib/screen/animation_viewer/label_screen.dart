import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/screen/animation_viewer/provider/selected_label.dart';

class LabelScreen extends ConsumerWidget {
  const LabelScreen({
    super.key,
    required this.label,
  });

  final List<String> label;

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return ListView.builder(
      itemCount: label.length,
      itemBuilder: (context, index) => Tooltip(
        message: label[index],
        child: Card(
          child: ListTile(
            leading: const Icon(Symbols.animation),
            trailing: IconButton(
              icon: const Icon(Symbols.play_arrow),
              onPressed: () {
                ref.read(selectedLabel.notifier).setLabel(label[index]);
              },
            ),
            title: Text(label[index]),
          ),
        ),
      ),
    );
  }
}
