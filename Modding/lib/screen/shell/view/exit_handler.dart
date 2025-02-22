import 'package:flutter/material.dart';
import 'package:sen/i18n/app_localizations.dart';

class ExitHandler extends StatelessWidget {
  const ExitHandler({
    super.key,
    required this.finished,
    required this.child,
  });

  final bool finished;

  final Widget child;

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return PopScope(
      canPop: finished,
      onPopInvokedWithResult: (didPop, result) async {
        if (!finished) {
          await showDialog(
            context: context,
            builder: (context) => AlertDialog(
              title: Text(los.invalid_request),
              content: Text(los.shell_request_behavior),
              actions: [
                TextButton(
                  onPressed: () {
                    Navigator.of(context).pop();
                  },
                  child: Text(los.okay),
                ),
              ],
            ),
          );
        }
      },
      child: child,
    );
  }
}
