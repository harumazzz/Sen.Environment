import 'package:flutter/material.dart';
import 'package:sen/i18n/app_localizations.dart';

class FinishedStage extends StatelessWidget {
  final VoidCallback onConfirm;
  final VoidCallback onLeave;

  const FinishedStage({
    super.key,
    required this.onConfirm,
    required this.onLeave,
  });

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;

    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      spacing: 10.0,
      children: [
        Text(los.execute_again),
        Row(
          spacing: 10.0,
          children: [
            Expanded(
              child: ElevatedButton(
                onPressed: onConfirm,
                child: Padding(
                  padding: const EdgeInsets.all(16.0),
                  child: Text(los.yes),
                ),
              ),
            ),
            Expanded(
              child: ElevatedButton(
                onPressed: onLeave,
                child: Padding(
                  padding: const EdgeInsets.all(16.0),
                  child: Text(los.no),
                ),
              ),
            ),
          ],
        ),
      ],
    );
  }
}
