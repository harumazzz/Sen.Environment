import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../bloc/add_option_bloc/add_option_bloc.dart';
import '../../bloc/argument_bloc/argument_bloc.dart';
import '../../bloc/error_traceback_bloc/error_traceback_bloc.dart';
import '../../bloc/interaction_bloc/interaction_bloc.dart';
import '../../bloc/launch_status_bloc/launch_status_bloc.dart';
import '../../bloc/message_bloc/message_bloc.dart';
import '../../bloc/initial_directory_bloc/initial_directory_bloc.dart';
import '../../i18n/app_localizations.dart';
import '../../extension/context.dart';
import '../../widget/material_dialog.dart';
import 'interaction_bar.dart';
import 'message_box.dart';
import '../../service/file_helper.dart';
import '../../service/ui_helper.dart';
import '../../widget/hotkey.dart';

class ShellScreen extends StatelessWidget {
  const ShellScreen({super.key});

  Future<void> _onSave(BuildContext context, String value) async {
    void closeDialog() => Navigator.of(context).pop();
    var file = await FileHelper.saveFile(
      initialDirectory: () {
        final state = context.read<InitialDirectoryBloc>().state;
        return state is InitialDirectoryLoaded ? state.initialDirectory : null;
      }(),
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

  Future<void> _showLog(BuildContext context, String value) async {
    final los = AppLocalizations.of(context)!;
    clearEvent() => context.read<AddOptionBloc>().add(const ClearOptionEvent());
    await showDialog(
      context: context,
      builder: (context) {
        return MaterialDialog(
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
        BlocProvider<MessageBloc>(create: (_) => MessageBloc()),
        BlocProvider<LaunchStatusBloc>(create: (_) => LaunchStatusBloc()),
        BlocProvider<InteractionBloc>(create: (_) => InteractionBloc()),
        BlocProvider<ErrorTracebackBloc>(create: (_) => ErrorTracebackBloc()),
        BlocProvider<AddOptionBloc>(create: (_) => AddOptionBloc()),
        BlocProvider<ArgumentBloc>(create: (_) => ArgumentBloc()),
      ],
      child: Builder(
        builder: (context) {
          return BlocBuilder<LaunchStatusBloc, LaunchStatusState>(
            builder: (context, state) {
              return HotkeyBuilder(
                child: PopScope(
                  canPop: !state.isRunning,
                  onPopInvokedWithResult: (didPop, result) async {
                    if (state.isRunning) {
                      await UIHelper.showSimpleDialog(
                        context: context,
                        title: context.los.invalid_request,
                        content: context.los.shell_request_behavior,
                      );
                    }
                  },
                  child: Scaffold(
                    appBar: UIHelper.ofMobile(
                      builder: () {
                        return AppBar(
                          title: Text(
                            context.los.shell,
                            style: const TextStyle(fontWeight: FontWeight.bold),
                          ),
                        );
                      },
                    ),
                    body: CustomShellBody(onLog: _showLog),
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

class CustomShellBody extends StatelessWidget {
  const CustomShellBody({super.key, required this.onLog});

  final Future<void> Function(BuildContext context, String value) onLog;

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        Expanded(
          child: BlocListener<AddOptionBloc, AddOptionState>(
            listener: (context, state) async {
              if (state is ExportLogState) {
                await onLog(context, state.value);
              }
            },
            child: const MessageBox(),
          ),
        ),
        const Padding(
          padding: EdgeInsets.symmetric(horizontal: 4.0, vertical: 4.0),
          child: InteractionBar(),
        ),
      ],
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<
        Future<void> Function(BuildContext context, String value)
      >.has('onLog', onLog),
    );
  }
}
