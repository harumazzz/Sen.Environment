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
    final theme = Theme.of(context);
    return ListView.builder(
      itemCount: label.length,
      itemBuilder: (context, index) => Card(
        elevation: 4.0,
        shadowColor: theme.colorScheme.shadow.withValues(alpha: 0.2),
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(32.0),
        ),
        margin: const EdgeInsets.symmetric(vertical: 4.0),
        child: ListTile(
          contentPadding: const EdgeInsets.symmetric(horizontal: 16.0),
          leading: Icon(
            Symbols.label_important,
            color: theme.colorScheme.primary,
          ),
          title: Text(
            label[index],
            style: theme.textTheme.titleMedium,
            overflow: TextOverflow.ellipsis,
          ),
          trailing: Tooltip(
            message: label[index],
            child: IconButton(
              icon: Icon(
                Symbols.play_arrow,
                color: theme.colorScheme.secondary,
              ),
              onPressed: () {
                context.read<SelectedLabelBloc>().add(SetLabelEvent(newLabel: label[index]));
              },
            ),
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
