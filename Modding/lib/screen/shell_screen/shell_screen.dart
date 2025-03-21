import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import '../../bloc/add_option_bloc/add_option_bloc.dart';
import '../../bloc/argument_bloc/argument_bloc.dart';
import '../../bloc/error_traceback_bloc/error_traceback_bloc.dart';
import '../../bloc/interaction_bloc/interaction_bloc.dart';
import '../../bloc/launch_status_bloc/launch_status_bloc.dart';
import '../../bloc/message_bloc/message_bloc.dart';
import '../../cubit/initial_directory_cubit/initial_directory_cubit.dart';
import '../../i18n/app_localizations.dart';
import '../../extension/context.dart';
import '../../widget/error_page.dart';
import '../../widget/material_dialog.dart';
import 'interaction_bar.dart';
import 'message_box.dart';
import '../../service/file_helper.dart';
import '../../service/ui_helper.dart';
import '../../widget/hotkey.dart';

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
            await UIHelper.showDetailDialog(
              context: context,
              title: Text(
                context.los.error,
                style: Theme.of(
                  context,
                ).textTheme.titleMedium?.copyWith(fontWeight: FontWeight.w600),
              ),
              content: ErrorPage(errors: state.errors),
            );
          },
        );
      },
    );
  }

  Future<void> _onSave(BuildContext context, String value) async {
    void closeDialog() => Navigator.of(context).pop();
    var file = await FileHelper.saveFile(
      initialDirectory:
          BlocProvider.of<InitialDirectoryCubit>(
            context,
          ).state.initialDirectory,
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
                    appBar: _buildAppBar(context),
                    body: _buildBody(),
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

  PreferredSizeWidget? _buildAppBar(BuildContext context) {
    return UIHelper.ofMobile(
      builder:
          () => AppBar(
            title: Text(
              context.los.shell,
              style: const TextStyle(fontWeight: FontWeight.bold),
            ),
          ),
    );
  }

  Widget _buildBody() {
    return Column(
      children: [
        Expanded(
          child: BlocListener<AddOptionBloc, AddOptionState>(
            listener: (context, state) async {
              if (state is ExportLogState) {
                await _showLog(context, state.value);
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
}
