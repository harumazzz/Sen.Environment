import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/bloc/load_script_bloc/load_script_bloc.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/screen/shell_screen/shell_screen.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/service/ui_helper.dart';
import 'package:sen/service/windows_helper.dart';

@immutable
class JavaScriptCard extends StatelessWidget {
  const JavaScriptCard({
    super.key,
    required this.item,
    required this.toolChain,
  });

  final Script item;
  final String toolChain;

  Color? _iconColor(BuildContext context) {
    return Theme.of(context).brightness == Brightness.dark
        ? Colors.yellowAccent.withValues(alpha: 0.8)
        : Theme.of(context).colorScheme.primary;
  }

  List<String> _makeArguments() {
    return [
      '-method',
      'js.evaluate',
      '-source',
      item.path.replaceFirst(
        '.',
        '$toolChain/Script/Helper',
      ),
    ];
  }

  Future<void> _showDialog(
    BuildContext context,
    String title,
    String message,
  ) async {
    await UIHelper.showSimpleDialog(
      context: context,
      title: title,
      content: message,
    );
  }

  Future<void> _runAsLauncher(
    BuildContext context,
  ) async {
    if (!Platform.isWindows) {
      return await _runAsShell(context);
    }
    Future<void> showError(Object e) async {
      final los = AppLocalizations.of(context)!;
      await _showDialog(context, los.error, e.toString());
    }

    Future<void> showSuccess() async {
      final los = AppLocalizations.of(context)!;
      await _showDialog(context, los.done, los.spawn_success(item.name));
    }

    String launcher() => '${context.read<SettingsCubit>().state.toolChain}/Launcher.exe';
    try {
      await WindowsHelper.runLauncher(
        argument: '${launcher()} ${_makeArguments().join(' ')}',
      );
      await showSuccess();
    } catch (e) {
      await showError(e);
    }
  }

  Future<void> _runAsShell(
    BuildContext context,
  ) async {
    await Navigator.of(context).push(
      MaterialPageRoute(
        builder: (context) => const ShellScreen(),
      ),
    );
  }

  void _onTap(
    BuildContext context,
  ) async {
    if (context.mounted) {
      await _onConfirm(context, () async {
        if (BlocProvider.of<SettingsCubit>(context).state.jsRunAsLauncher) {
          await _runAsLauncher(context);
        } else {
          await _runAsShell(context);
        }
      });
    }
  }

  Future<void> _onConfirm(
    BuildContext context,
    Future<dynamic> Function() function,
  ) async {
    final los = AppLocalizations.of(context)!;
    if (BlocProvider.of<SettingsCubit>(context).state.jsShowConfirmDialog) {
      await UIHelper.showFlutterDialog(
        context: context,
        child: UIHelper.buildDialog(
          title: Text(los.confirmation),
          content: Text(los.do_you_wish_to_run(item.name)),
          actions: [
            TextButton(
              onPressed: () => Navigator.of(context).pop(),
              child: Text(los.cancel),
            ),
            TextButton(
              onPressed: () async {
                Navigator.of(context).pop();
                await function();
              },
              child: Text(los.okay),
            ),
          ],
        ),
      );
      return;
    }
    return await function();
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    final theme = Theme.of(context);
    final cardColor = theme.colorScheme.surfaceContainerHighest;
    final textColor = theme.colorScheme.onSurface;
    return Card(
      elevation: 4.0,
      color: cardColor,
      surfaceTintColor: cardColor,
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(16.0),
      ),
      child: InkWell(
        borderRadius: BorderRadius.circular(16.0),
        onTap: () => _onTap(context),
        child: Padding(
          padding: const EdgeInsets.symmetric(horizontal: 8.0, vertical: 8.0),
          child: Row(
            children: [
              Icon(Symbols.javascript, size: 32.0, color: _iconColor(context)),
              const SizedBox(width: 16.0),
              Expanded(
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  spacing: 4.0,
                  children: [
                    Text(
                      item.name,
                      style: theme.textTheme.titleMedium?.copyWith(
                        fontWeight: FontWeight.bold,
                        color: textColor,
                      ),
                    ),
                    Text(
                      item.description,
                      style: theme.textTheme.bodyMedium?.copyWith(
                        color: theme.colorScheme.onSurfaceVariant,
                      ),
                    ),
                  ],
                ),
              ),
              Tooltip(
                message: los.js_execute,
                child: Icon(Icons.play_arrow_rounded, size: 28, color: theme.colorScheme.primary),
              ),
              const SizedBox(width: 16.0),
            ],
          ),
        ),
      ),
    );
  }
}
