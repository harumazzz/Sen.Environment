import 'package:flutter/material.dart';
import 'package:sen/screen/map_editor/app/l10n/l10n.dart';

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
      actions: [
        ElevatedButton(
          style: ButtonStyle(
              shape: WidgetStateProperty.all(RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(12)))),
          child: Text(los.cancel),
          onPressed: () => Navigator.of(context).pop(false),
        ),
        ElevatedButton(
          style: ButtonStyle(
              shape: WidgetStateProperty.all(RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(12)))),
          child: Text(los.yes),
          onPressed: () => Navigator.of(context).pop(true),
        ),
      ],
    );
  }
}
