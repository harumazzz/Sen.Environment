import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:page_transition/page_transition.dart';
import 'package:sen/bloc/load_script_bloc/load_script_bloc.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/screen/shell/shell_screen.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
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

  Color? _color(
    BuildContext context,
  ) {
    return Theme.of(context).brightness == Brightness.dark ? Colors.yellowAccent.withValues(alpha: 0.8) : null;
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
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(title),
        content: Text(message),
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
      PageTransition(
        duration: const Duration(milliseconds: 300),
        type: PageTransitionType.rightToLeft,
        child: ShellScreen(
          arguments: _makeArguments(),
          launchImmediately: true,
        ),
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
      await showDialog(
        context: context,
        builder: (context) => AlertDialog(
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
    return Card(
      child: ListTile(
        leading: Icon(Symbols.javascript, size: 28, color: _color(context)),
        title: Text(
          item.name,
          style: Theme.of(context).textTheme.labelMedium?.copyWith(fontWeight: FontWeight.bold),
        ),
        subtitle: Text(
          item.description,
          style: Theme.of(context).textTheme.labelSmall,
        ),
        onTap: () => _onTap(context),
        trailing: Tooltip(
          message: los.js_execute,
          child: const Icon(Symbols.arrow_forward),
        ),
      ),
    );
  }
}
