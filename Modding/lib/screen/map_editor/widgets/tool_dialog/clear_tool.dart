import 'package:flutter/material.dart';
import 'package:sen/extension/l10n.dart';

class ClearToolWidget extends StatefulWidget {
  const ClearToolWidget({super.key});

  @override
  State<StatefulWidget> createState() => _ClearToolState();
}

class _ClearToolState extends State<ClearToolWidget> {
  @override
  Widget build(BuildContext context) {
    final los = context.los;
    return AlertDialog(
      title: Text(los.confirm_delete),
      content: Text(los.confirm_delete_description),
      backgroundColor: Theme.of(context).colorScheme.onInverseSurface,
      actions: [
        TextButton(
          child: Text(los.cancel),
          onPressed: () => Navigator.of(context).pop(false),
        ),
        TextButton(
          child: Text(los.yes), //TODO: chang locale "yes" -> "Yes"
          onPressed: () => Navigator.of(context).pop(true),
        ),
      ],
    );
  }
}
