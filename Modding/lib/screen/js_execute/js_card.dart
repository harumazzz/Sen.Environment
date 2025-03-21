import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../bloc/load_script_bloc/load_script_bloc.dart';
import '../../cubit/settings_cubit/settings_cubit.dart';
import '../../extension/context.dart';
import '../../extension/platform.dart';
import '../shell_screen/shell_screen.dart';
import '../../i18n/app_localizations.dart';
import '../../service/ui_helper.dart';
import '../../service/windows_helper.dart';

@immutable
class JsCard extends StatelessWidget {
  const JsCard({super.key, required this.item, required this.toolChain});

  final Script item;

  final String toolChain;

  List<String> _makeArguments() {
    return [
      '-method',
      'js.evaluate',
      '-source',
      item.path.replaceFirst('.', '$toolChain/Script/Helper'),
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

  Future<void> _runAsLauncher(BuildContext context) async {
    if (!CurrentPlatform.isWindows) {
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

    String launcher() =>
        '${context.read<SettingsCubit>().state.toolChain}/Launcher.exe';
    try {
      await WindowsHelper.runLauncher(
        argument: '${launcher()} ${_makeArguments().join(' ')}',
      );
      Future.delayed(const Duration(milliseconds: 300), () async {
        await showSuccess();
      });
    } catch (e) {
      await showError(e);
    }
  }

  Future<void> _runAsShell(BuildContext context) async {
    await Navigator.of(
      context,
    ).push(MaterialPageRoute(builder: (context) => const ShellScreen()));
  }

  void _onTap(BuildContext context) async {
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
          title: Text(
            los.confirmation,
            style: Theme.of(
              context,
            ).textTheme.titleMedium?.copyWith(fontWeight: FontWeight.w600),
          ),
          content: Text(
            los.do_you_wish_to_run(item.name),
            style: Theme.of(context).textTheme.titleSmall,
          ),
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

  Future<void> _showContextMenu(
    TapDownDetails details,
    BuildContext context,
  ) async {
    final overlay = Overlay.of(context).context.findRenderObject() as RenderBox;
    await showMenu(
      context: context,
      position: RelativeRect.fromRect(
        details.globalPosition & const Size(40, 40),
        Offset.zero & overlay.size,
      ),
      items: [
        PopupMenuItem(
          onTap: () {
            _onTap(context);
            ContextMenuController.removeAny();
          },
          child: Text(context.los.play),
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);
    final cardColor = theme.colorScheme.surfaceContainerHighest;
    final textColor = theme.colorScheme.onSurface;
    return GestureDetector(
      onSecondaryTapDown:
          (details) async => await _showContextMenu(details, context),
      child: Card(
        elevation: 4,
        color: cardColor,
        surfaceTintColor: cardColor,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(20.0),
        ),
        child: InkWell(
          borderRadius: BorderRadius.circular(20.0),
          onTap: () => _onTap(context),
          child: Padding(
            padding: const EdgeInsets.symmetric(
              horizontal: 16.0,
              vertical: 16.0,
            ),
            child: Row(
              children: [
                _buildLeadingIcon(context),
                const SizedBox(width: 16.0),
                Expanded(child: _buildTextContent(theme, textColor)),
                _buildActionIcon(theme, context),
              ],
            ),
          ),
        ),
      ),
    );
  }

  Widget _buildLeadingIcon(BuildContext context) {
    return Container(
      padding: const EdgeInsets.all(8.0),
      decoration: BoxDecoration(
        color: Theme.of(context).colorScheme.primaryContainer,
        shape: BoxShape.circle,
      ),
      child: Icon(
        Symbols.javascript,
        size: 32.0,
        color: Theme.of(context).colorScheme.onPrimaryContainer,
      ),
    );
  }

  Widget _buildTextContent(ThemeData theme, Color textColor) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          item.name,
          style: theme.textTheme.titleMedium?.copyWith(
            fontWeight: FontWeight.bold,
            color: textColor,
          ),
        ),
        const SizedBox(height: 4.0),
        Text(
          item.description,
          style: theme.textTheme.bodyMedium?.copyWith(
            color: theme.colorScheme.onSurfaceVariant,
          ),
        ),
      ],
    );
  }

  Widget _buildActionIcon(ThemeData theme, BuildContext context) {
    return IconButton(
      tooltip: context.los.js_execute,
      icon: const Icon(Icons.play_arrow_rounded),
      iconSize: 28,
      color: theme.colorScheme.primary,
      onPressed: () => _onTap(context),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Script>('item', item));
    properties.add(StringProperty('toolChain', toolChain));
  }
}
