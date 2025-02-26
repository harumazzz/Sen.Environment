import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:sen/bloc/add_option_bloc/add_option_bloc.dart';
import 'package:sen/bloc/error_traceback_bloc/error_traceback_bloc.dart';
import 'package:sen/bloc/interaction_bloc/interaction_bloc.dart';
import 'package:sen/bloc/launch_status_bloc/launch_status_bloc.dart';
import 'package:sen/bloc/message_bloc/message_bloc.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/extension/l10n.dart';
import 'package:sen/screen/shell/interaction_bar.dart';
import 'package:sen/screen/shell/message_box.dart';
import 'package:sen/service/file_helper.dart';
import 'package:sen/service/ui_helper.dart';
import 'package:sen/widget/hotkey.dart';

class ShellScreen extends StatelessWidget {
  const ShellScreen({super.key});

  Widget _buildStackButton(BuildContext context) {
    return BlocBuilder<ErrorTracebackBloc, ErrorTracebackState>(
      builder: (context, state) {
        if (state is! ErrorTracebackMessage) {
          return const SizedBox.shrink();
        }
        return FloatingActionButton(
          heroTag: 'stacktrace',
          child: const Icon(Symbols.dangerous),
          onPressed: () async {
            final stack = state.message.subtitle!.split('\n');
            await UIHelper.showDetailDialog(
              context: context,
              title: Text(state.message.title),
              content: SingleChildScrollView(
                child: Column(
                  mainAxisSize: MainAxisSize.min,
                  crossAxisAlignment: CrossAxisAlignment.start,
                  mainAxisAlignment: MainAxisAlignment.start,
                  children: [
                    ...stack.map((e) => _buildStackMessage(context, e)),
                  ],
                ),
              ),
            );
          },
        );
      },
    );
  }

  Widget _buildStackMessage(BuildContext context, String message) {
    final match = RegExp(r'(.+?)\s+(.+?)\s+\((.+?)\)').firstMatch(message.trim());
    final functionName = match?.group(2) ?? '';
    final filePath = match?.group(3) ?? '';
    return Padding(
      padding: const EdgeInsets.symmetric(vertical: 4),
      child: Row(
        crossAxisAlignment: CrossAxisAlignment.center,
        children: [
          const Icon(
            Symbols.dangerous,
            size: 24.0,
            color: Colors.red,
          ),
          const SizedBox(width: 8.0),
          Expanded(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  functionName,
                  style: const TextStyle(
                    fontWeight: FontWeight.bold,
                    fontSize: 14,
                    color: Colors.red,
                  ),
                  softWrap: true,
                ),
                if (filePath.isNotEmpty)
                  Text(
                    filePath,
                    style: TextStyle(
                      fontSize: 12,
                      color: Theme.of(context).colorScheme.secondary,
                    ),
                    softWrap: true,
                  ),
              ],
            ),
          ),
        ],
      ),
    );
  }

  Future<void> _onSave(
    BuildContext context,
    String value,
  ) async {
    void closeDialog() => Navigator.of(context).pop();
    var file = await FileHelper.saveFile(
      initialDirectory: BlocProvider.of<InitialDirectoryCubit>(context).state.initialDirectory,
      suggestedName: 'log.json',
    );
    if (file != null) {
      if (RegExp(r'\.json$', caseSensitive: false).hasMatch(file)) {
        file += '.json';
      }
      FileHelper.writeFile(source: file, data: value);
      closeDialog();
    }
  }

  Future<void> _showLog(
    BuildContext context,
    String value,
  ) async {
    final los = AppLocalizations.of(context)!;
    clearEvent() => context.read<AddOptionBloc>().add(const ClearOptionEvent());
    await showDialog(
      context: context,
      builder: (context) {
        return AlertDialog(
          title: Text(los.export_log),
          content: Text(value),
          actions: [
            TextButton(
              onPressed: () => _onSave(context, value),
              child: Text(los.save),
            ),
            TextButton(
              child: Text(los.okay),
              onPressed: () {
                Navigator.of(context).pop();
              },
            ),
          ],
        );
      },
    );
    clearEvent();
  }

  @override
  Widget build(BuildContext context) {
    return MultiBlocProvider(
      providers: [
        BlocProvider<MessageBloc>(
          create: (context) => MessageBloc(),
        ),
        BlocProvider<LaunchStatusBloc>(
          create: (context) => LaunchStatusBloc(),
        ),
        BlocProvider<InteractionBloc>(
          create: (context) => InteractionBloc(),
        ),
        BlocProvider<ErrorTracebackBloc>(
          create: (context) => ErrorTracebackBloc(),
        ),
        BlocProvider<AddOptionBloc>(
          create: (context) => AddOptionBloc(),
        ),
      ],
      child: Builder(
        builder: (context) {
          return BlocBuilder<LaunchStatusBloc, LaunchStatusState>(
            builder: (context, state) {
              return HotkeyBuilder(
                child: PopScope(
                  canPop: !state.isRunning,
                  onPopInvokedWithResult: (didPop, result) async {
                    final canPop = !state.isRunning;
                    if (!canPop) {
                      await UIHelper.showSimpleDialog(
                        context: context,
                        title: context.los.invalid_request,
                        content: context.los.shell_request_behavior,
                      );
                    }
                  },
                  child: Scaffold(
                    appBar: AppBar(
                      title: Text(context.los.shell),
                    ),
                    body: BlocListener<AddOptionBloc, AddOptionState>(
                      listener: (context, state) async {
                        if (state is ExportLogState) {
                          await _showLog(context, state.value);
                        }
                      },
                      child: const MessageBox(),
                    ),
                    bottomNavigationBar: const InteractionBar(),
                    floatingActionButton: _buildStackButton(context),
                  ),
                ),
              );
            },
          );
        },
      ),
    );
  }
}
