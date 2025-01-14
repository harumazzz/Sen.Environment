import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/bloc/selected_label_bloc/selected_label_bloc.dart';

class LabelScreen extends StatelessWidget {
  const LabelScreen({
    super.key,
    required this.label,
  });

  final List<String> label;

  @override
  Widget build(BuildContext context) {
    return ListView.builder(
      itemCount: label.length,
      itemBuilder: (context, index) => Tooltip(
        message: label[index],
        child: Card(
          child: ListTile(
            contentPadding: const EdgeInsets.symmetric(horizontal: 8.0),
            leading: const Icon(Symbols.animation),
            trailing: IconButton(
              icon: const Icon(Symbols.play_arrow),
              onPressed: () {
                context.read<SelectedLabelBloc>().add(SetLabelEvent(newLabel: label[index]));
              },
            ),
            title: Text(label[index]),
          ),
        ),
      ),
    );
  }
}
