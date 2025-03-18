import 'package:flutter/material.dart';
import '../../../../extension/context.dart';
import '../../../../widget/material_dialog.dart';

class ClearToolWidget extends StatefulWidget {
  const ClearToolWidget({super.key});

  @override
  State<StatefulWidget> createState() => _ClearToolState();
}

class _ClearToolState extends State<ClearToolWidget> {
  @override
  Widget build(BuildContext context) {
    final los = context.los;
    return MaterialDialog(
      title: Text(los.confirm_delete),
      content: Text(los.confirm_delete_description),
      actions: [
        TextButton(
          child: Text(los.cancel),
          onPressed: () => Navigator.of(context).pop(false),
        ),
        TextButton(
          child: Text(los.yes),
          onPressed: () => Navigator.of(context).pop(true),
        ),
      ],
    );
  }
}
