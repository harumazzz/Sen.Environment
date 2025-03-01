import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/bloc/selected_label_bloc/selected_label_bloc.dart';
import 'package:sen/extension/platform.dart';

class LabelScreen extends StatelessWidget {
  const LabelScreen({
    super.key,
    required this.label,
  });

  final List<String> label;

  Widget _wrappableOf({
    required Widget child,
  }) {
    if (CurrentPlatform.isDesktop) {
      return child;
    }
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: child,
    );
  }

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);
    return _wrappableOf(
      child: ListView.builder(
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
              style: theme.textTheme.labelLarge?.copyWith(
                color: theme.colorScheme.onSurface,
                fontWeight: FontWeight.w500,
              ),
              overflow: TextOverflow.ellipsis,
              maxLines: 4,
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
      ),
    );
  }
}
