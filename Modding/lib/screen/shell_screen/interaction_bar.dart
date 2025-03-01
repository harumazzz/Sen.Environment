import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/bloc/error_traceback_bloc/error_traceback_bloc.dart';
import 'package:sen/bloc/interaction_bloc/interaction_bloc.dart';
import 'package:sen/bloc/launch_status_bloc/launch_status_bloc.dart';
import 'package:sen/bloc/message_bloc/message_bloc.dart';
import 'package:sen/bridge/client.dart';
import 'package:sen/constant/build_distribution.dart';
import 'package:sen/model/message.dart';
import 'package:sen/model/select_option.dart';
import 'package:sen/screen/shell_screen/boolean_bar.dart';
import 'package:sen/screen/shell_screen/empty_bar.dart';
import 'package:sen/screen/shell_screen/enumeration_bar.dart';
import 'package:sen/screen/shell_screen/idle_bar.dart';
import 'package:sen/screen/shell_screen/input_bar.dart';
import 'package:sen/screen/shell_screen/loading_bar.dart';
import 'package:sen/service/ui_helper.dart';

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
  }

  @override
  void dispose() {
    super.dispose();
  }

  Widget _buildWrapper({
    required Widget child,
  }) {
    return SafeArea(
      child: Container(
        margin: const EdgeInsets.symmetric(
          vertical: 6.0,
          horizontal: 8.0,
        ),
        child: child,
      ),
    );
  }

  final List<String> arguments = const [
    'C:/Users/Admin/Documents/Sen.Environment/Kernel.dll',
    'C:/Users/Admin/Documents/Sen.Environment/Kernel.dll',
    'C:/Users/Admin/Documents/Sen.Environment/Script/main.js',
  ];

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
          return EnumerationBar(
            onSelect: _onEnumerationSelect,
          );
        }
        return const EmptyBar();
      },
    );
  }

  void _onLaunch(
    BuildContext context,
  ) {
    context.read<LaunchStatusBloc>().add(LaunchStatusBegin(client: this, arguments: arguments));
  }

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<LaunchStatusBloc, LaunchStatusState>(
      builder: (context, state) {
        if (state is LaunchStatusInitial || state is LaunchStatusEnd) {
          return _buildWrapper(
            child: IdleBar(
              onLaunch: () => _onLaunch(context),
            ),
          );
        }
        return _buildWrapper(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            mainAxisAlignment: MainAxisAlignment.start,
            mainAxisSize: MainAxisSize.min,
            spacing: 10.0,
            children: [
              const LoadingBar(),
              _buildInteraction(),
            ],
          ),
        );
      },
    );
  }

  Future<void> _onDisplay(
    List<String> arguments,
  ) async {
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
          message: Message(
            title: title,
            color: color,
            subtitle: subtitle,
          ),
        ),
      );
    }

    await Future.delayed(const Duration(milliseconds: 150), addMessage);
  }

  Future<void> _onInputString(
    List<String> destination,
  ) async {
    _completer = Completer<String?>();
    context.read<InteractionBloc>().add(StringInputEvent(completer: _completer));
    final result = await _completer.future;
    if (result != null) {
      destination.add(result);
    }
  }

  Future<void> _onInputBoolean(
    List<String> destination,
  ) async {
    _completer = Completer<String?>();
    context.read<InteractionBloc>().add(BooleanInputEvent(completer: _completer));
    final result = await _completer.future;
    if (result != null) {
      destination.add(result);
    }
  }

  void _onEnumerationSelect() {
    UIHelper.showDropDownModal<SelectOption>(
      context: context,
      data: UIHelper.makeDefaultItems<SelectOption>(data: _option!),
      onTap: (selectedItems) async {
        final result = UIHelper.toItemList<SelectOption>(selectedItems);
        context.read<InteractionBloc>().add(EnumerationSelectCompleteEvent(value: result[0].option));
      },
    );
  }

  Future<void> _onSelectEnumeration(
    List<String> selectedOptions,
    List<String> destination,
  ) async {
    _completer = Completer<String?>();
    _option = UIHelper.makeEnumerationOption(selectedOptions);
    context.read<InteractionBloc>().add(EnumerationSelectEvent(completer: _completer));
    _onEnumerationSelect();
    final result = await _completer.future;
    if (result != null) {
      destination.add(result);
      _option = null;
    }
  }

  void _onDisplayStack(
    String title,
    String subtitle,
  ) {
    BlocProvider.of<ErrorTracebackBloc>(context).add(
      ErrorTracebackMessageEvent(
        message: Message(
          title: title,
          subtitle: subtitle,
        ),
      ),
    );
  }

  @override
  Future<List<String>> callback(
    List<String> arguments,
  ) async {
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
        await _onSelectEnumeration([...arguments.take(0), ...arguments.skip(1)], result);
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
    }
    return result;
  }

  @override
  Future<void> finish() async {
    onComplete() {
      BlocProvider.of<LaunchStatusBloc>(context).add(const LaunchStatusComplete());
    }

    await Future.delayed(const Duration(milliseconds: 100), onComplete);
  }

  @override
  Future<void> start() async {
    BlocProvider.of<MessageBloc>(context).add(const ClearMessage());
    BlocProvider.of<ErrorTracebackBloc>(context).add(const ErrorTracebackClearEvent());
  }
}
