import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../bloc/argument_bloc/argument_bloc.dart';
import '../../bloc/error_traceback_bloc/error_traceback_bloc.dart';
import '../../bloc/interaction_bloc/interaction_bloc.dart';
import '../../bloc/launch_status_bloc/launch_status_bloc.dart';
import '../../bloc/message_bloc/message_bloc.dart';
import '../../bridge/client.dart';
import '../../constant/build_distribution.dart';
import '../../cubit/initial_directory_cubit/initial_directory_cubit.dart';
import '../../cubit/settings_cubit/settings_cubit.dart';
import '../../extension/context.dart';
import '../../model/message.dart';
import '../../model/select_option.dart';
import 'boolean_bar.dart';
import 'empty_bar.dart';
import 'enumeration_bar.dart';
import 'enumeration_page.dart';
import 'idle_bar.dart';
import 'input_bar.dart';
import 'loading_bar.dart';
import '../../service/file_helper.dart';
import '../../service/ui_helper.dart';

class InteractionBar extends StatefulWidget {
  const InteractionBar({super.key});

  @override
  State<InteractionBar> createState() => _InteractionBarState();
}

class _InteractionBarState extends State<InteractionBar> implements Client {
  late Completer<String?> _completer;
  List<SelectOption>? _option;

  @override
  void initState() {
    super.initState();
    WidgetsBinding.instance.addPostFrameCallback((_) {
      if (context.read<SettingsCubit>().state.shellLaunchImmediately) {
        _onLaunch(context);
      }
    });
  }

  @override
  void dispose() {
    super.dispose();
  }

  Widget _buildWrapper({required Widget child}) {
    return SafeArea(
      child: Container(
        margin: const EdgeInsets.symmetric(vertical: 6.0, horizontal: 8.0),
        child: child,
      ),
    );
  }

  Widget _buildInteraction() {
    return BlocBuilder<InteractionBloc, InteractionState>(
      builder: (context, state) {
        if (state is StringInputState) {
          return const InputBar();
        }
        if (state is BooleanInputState) {
          return const BooleanBar();
        }
        if (state is EnumerationSelectState) {
          return EnumerationBar(onSelect: _onEnumerationSelect);
        }
        return const EmptyBar();
      },
    );
  }

  void _onLaunch(BuildContext context) {
    context.read<LaunchStatusBloc>().add(
      LaunchStatusBegin(
        client: this,
        setting: context.read<SettingsCubit>(),
        arguments: context.read<ArgumentBloc>().state.value,
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<LaunchStatusBloc, LaunchStatusState>(
      builder: (context, state) {
        if (state is LaunchStatusInitial || state is LaunchStatusEnd) {
          return _buildWrapper(
            child: IdleBar(onLaunch: () => _onLaunch(context)),
          );
        }
        return _buildWrapper(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            mainAxisSize: MainAxisSize.min,
            spacing: 10.0,
            children: [const LoadingBar(), _buildInteraction()],
          ),
        );
      },
    );
  }

  Future<void> _onDisplay(List<String> arguments) async {
    final title = arguments[1];
    var color = null as String?;
    var subtitle = null as String?;
    if (arguments.length > 2) {
      color = arguments[2];
    }
    if (arguments.length > 3) {
      subtitle = arguments[3];
    }
    addMessage() {
      BlocProvider.of<MessageBloc>(context).add(
        AddMessage(
          message: Message(title: title, color: color, subtitle: subtitle),
        ),
      );
    }

    addMessage();
  }

  Future<void> _onInputString(List<String> destination) async {
    _completer = Completer<String?>();
    context.read<InteractionBloc>().add(
      StringInputEvent(completer: _completer),
    );
    final result = await _completer.future;
    if (result != null) {
      destination.add(result);
    }
  }

  Future<void> _onInputBoolean(List<String> destination) async {
    _completer = Completer<String?>();
    context.read<InteractionBloc>().add(
      BooleanInputEvent(completer: _completer),
    );
    final result = await _completer.future;
    if (result != null) {
      destination.add(result);
    }
  }

  Future<void> _onEnumerationSelect() async {
    String messageOf({required bool Function(Message) test}) =>
        context.read<MessageBloc>().state.lastWhere(test).subtitle!;
    await showModalBottomSheet(
      context: context,
      builder: (_) {
        return BlocProvider.value(
          value: context.read<InteractionBloc>(),
          child: EnumerationPage(
            title: messageOf(test: (message) => message.subtitle != null),
            options: _option,
            onSelected: (selectedOption) {
              context.read<InteractionBloc>().add(
                EnumerationSelectCompleteEvent(value: selectedOption.option),
              );
            },
          ),
        );
      },
    );
  }

  Future<void> _onSelectEnumeration(
    List<String> selectedOptions,
    List<String> destination,
  ) async {
    _completer = Completer<String?>();
    _option = UIHelper.makeEnumerationOption(selectedOptions);
    context.read<MessageBloc>().add(RegisterContextMenu(options: _option!));
    context.read<InteractionBloc>().add(
      EnumerationSelectEvent(completer: _completer),
    );
    await _onEnumerationSelect();
    final result = await _completer.future;
    if (result != null) {
      destination.add(result);
      _option = null;
    }
  }

  void _onDisplayStack(String title, String subtitle) {
    BlocProvider.of<ErrorTracebackBloc>(context).add(
      ErrorTracebackMessageEvent(
        message: Message(title: title, subtitle: subtitle),
      ),
    );
  }

  Future<void> _onPickEvent(
    List<String> destination,
    Future<void> Function({String? initialDirectory}) onPick,
  ) async {
    _completer = Completer<String?>();
    context.read<InteractionBloc>().add(
      PickLocalStorage(completer: _completer),
    );
    final initialDirectory =
        context.read<InitialDirectoryCubit>().state.initialDirectory;
    await onPick(initialDirectory: initialDirectory);
    final result = await _completer.future;
    if (result != null) {
      destination.add(result);
    }
  }

  @override
  Future<List<String>> callback(List<String> arguments) async {
    final result = <String>[];
    switch (arguments[0]) {
      case 'display':
        await _onDisplay(arguments);
        break;
      case 'input_string':
        await _onInputString(result);
        break;
      case 'input_boolean':
        await _onInputBoolean(result);
        break;
      case 'input_enumeration':
        final enumerations = [...arguments.take(0), ...arguments.skip(1)];
        await _onSelectEnumeration(enumerations, result);
        break;
      case 'display_stack':
        _onDisplayStack(arguments[1], arguments[2]);
        break;
      case 'is_gui':
        result.add(BuildDistribution.kIsGui);
        break;
      case 'version':
        result.add(BuildDistribution.kVersion);
        break;
      case 'pick_file':
        await _onPickEvent(result, _buildCompleteEvent(FileHelper.uploadFile));
        break;
      case 'pick_directory':
        await _onPickEvent(
          result,
          _buildCompleteEvent(FileHelper.uploadDirectory),
        );
        break;
    }
    return result;
  }

  Future<void> Function({String? initialDirectory}) _buildCompleteEvent(
    Future<String?> Function({String? initialDirectory}) onPick,
  ) {
    return ({String? initialDirectory}) async {
      Future<void> showDialog(String result) async =>
          await UIHelper.showSimpleDialog(
            context: context,
            title: context.los.done,
            content: result,
          );
      final result = await onPick(initialDirectory: initialDirectory);
      if (result != null) {
        await showDialog(result);
      }
      _completer.complete(result);
    };
  }

  @override
  Future<void> finish() async {
    onComplete() {
      BlocProvider.of<LaunchStatusBloc>(
        context,
      ).add(const LaunchStatusComplete());
    }

    await Future.delayed(const Duration(milliseconds: 100), onComplete);
  }

  @override
  Future<void> start() async {
    BlocProvider.of<MessageBloc>(context).add(const ClearMessage());
    BlocProvider.of<ErrorTracebackBloc>(
      context,
    ).add(const ErrorTracebackClearEvent());
  }
}
