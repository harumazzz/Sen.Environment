import 'dart:async';
import 'dart:ffi';
import 'package:flutter/material.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:sen/model/api.dart';
import 'package:sen/model/message.dart';
import 'package:sen/screen/shell/controller/client_interaction_controller.dart';
import 'package:sen/screen/shell/view/boolean_stage.dart';
import 'package:sen/screen/shell/view/client_view.dart';
import 'package:sen/screen/shell/view/drop_handler.dart';
import 'package:sen/screen/shell/view/enumeration_stage.dart';
import 'package:sen/screen/shell/view/exit_handler.dart';
import 'package:sen/screen/shell/view/finished_stage.dart';
import 'package:sen/screen/shell/view/option_selector.dart';
import 'package:sen/screen/shell/controller/shell_controller.dart';
import 'package:sen/screen/shell/view/string_stage.dart';
import 'package:sen/service/file_service.dart';
import 'package:loading_animation_widget/loading_animation_widget.dart';

class ShellScreen extends ConsumerStatefulWidget {
  final List<String> arguments;

  const ShellScreen({
    super.key,
    required this.arguments,
  });

  @override
  ConsumerState<ShellScreen> createState() => _ShellScreenState();
}

class _ShellScreenState extends ConsumerState<ShellScreen> {
  TextEditingController? _inputController;
  late ScrollController _scrollController;
  bool _finished = false;
  late List<Message> _messages;
  String _stage = '';
  Completer<String?>? _completer;
  late ClientInteractionController _stateController;
  late List<String> _enumeration;
  late String _value;
  late ShellController _shellController;

  @override
  void initState() {
    _inputController = TextEditingController();
    _scrollController = ScrollController();
    _messages = [];
    _shellController = ShellController(
      setState: (function) {
        if (mounted) {
          setState(() => function());
        }
      },
      onAfter: _scrollToBottom,
      messages: _messages,
      onFinish: _setFinishState,
    );
    _stateController = ClientInteractionController(
      context: context,
      scrollController: _scrollController,
    );
    super.initState();
    _run(widget.arguments);
  }

  @override
  void dispose() {
    _inputController?.dispose();
    _scrollController.dispose();
    super.dispose();
  }

  void _exit() {
    Navigator.of(context).pop();
  }

  void _scrollToBottom() {
    if (_scrollController.hasClients) {
      _scrollController.animateTo(
        _scrollController.position.maxScrollExtent,
        duration: const Duration(milliseconds: 300),
        curve: Curves.easeOut,
      );
    }
  }

  void _setFinishState() {
    setState(() {
      _finished = true;
    });
    _scrollToBottom();
  }

  void _setPendingJob() {
    setState(() {
      _finished = false;
    });
    _scrollToBottom();
  }

  void _clearMessage() {
    _messages.clear();
    setState(() {});
  }

  Future<void> _inputString(
    Pointer<CStringView> destination,
  ) async {
    return await _stateController.inputString(destination, (completer) {
      _completer = completer;
      setState(() {
        _stage = 'input_string';
      });
    });
  }

  Future<void> _inputEnumeration(
    Pointer<CStringView> destination,
    List<String> restStatement,
  ) async {
    return await _stateController.inputEnumeration(destination, restStatement, (completer, value) {
      _completer = completer;
      _enumeration = restStatement;
      setState(() {
        _stage = 'input_enumeration';
        _value = value;
      });
    });
  }

  Future<void> _inputBoolean(
    Pointer<CStringView> destination,
  ) async {
    return await _stateController.inputBoolean(destination, (completer) {
      _completer = completer;
      setState(() {
        _stage = 'input_boolean';
      });
    });
  }

  void _run(
    List<String> arguments,
  ) async {
    _setPendingJob();
    _clearMessage();
    await _shellController.run(
      ref: ref,
      arguments: arguments,
      inputString: _inputString,
      inputEnumeration: _inputEnumeration,
      inputBoolean: _inputBoolean,
    );
    _setFinishState();
    return;
  }

  Widget _makeStage() {
    if (_finished) {
      _scrollToBottom();
      return _buildFinishedStage();
    }
    _scrollToBottom();
    switch (_stage) {
      case 'input_string':
        return _buildInputStringStage();
      case 'input_enumeration':
        return _buildInputEnumerationStage();
      case 'input_boolean':
        return _buildInputBooleanStage();
      default:
        return LoadingAnimationWidget.flickr(
          leftDotColor: Colors.blue,
          rightDotColor: Colors.pink,
          size: 40.0,
        );
    }
  }

  Widget _buildFinishedStage() {
    _scrollToBottom();
    return FinishedStage(
      onConfirm: () => _run(widget.arguments),
      onLeave: _exit,
    );
  }

  void _onSelect() async {
    return await showModalBottomSheet(
      context: context,
      builder: (BuildContext context) {
        return OptionSelector(
          onUploadFile: FileService.uploadFile,
          onUploadDirectory: FileService.uploadDirectory,
          inputController: _inputController,
          scrollToBottom: _scrollToBottom,
        );
      },
    );
  }

  Widget _buildInputStringStage() {
    return StringStage(
      controller: _inputController,
      onSelect: _onSelect,
      onSend: _onSendString,
    );
  }

  void _onSendString() {
    final data = _inputController!.text;
    _sendCompletedValue(data);
    _inputController!.text = '';
  }

  Widget _buildInputEnumerationStage() {
    _scrollToBottom();
    return EnumerationStage(
      enumeration: _enumeration,
      value: _value,
      onValueChanged: (String? newValue) {
        if (newValue != null) {
          setState(() {
            _value = newValue;
          });
        }
      },
      onSend: _onSendEnumeration,
    );
  }

  void _resetStage() {
    setState(() {
      _stage = 'default';
    });
  }

  void _sendCompletedValue(String? value) {
    _completer!.complete(value);
    _resetStage();
  }

  void _onSendEnumeration() {
    return _sendCompletedValue(_value);
  }

  Widget _buildInputBooleanStage() {
    _scrollToBottom();
    return BooleanStage(onSend: _onSendBoolean);
  }

  void _onSendBoolean(String currentOption) {
    final value = currentOption;
    final los = AppLocalizations.of(context)!;
    return _sendCompletedValue(value == los.yes ? '1' : '2');
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return ExitHandler(
      finished: _finished,
      child: Scaffold(
        appBar: AppBar(
          title: Text(los.shell),
        ),
        body: DropHandler(
          inputController: _inputController,
          child: ClientView(
            finished: _finished,
            messages: _messages,
            scrollController: _scrollController,
            makeStage: _makeStage,
            stage: _stage,
          ),
        ),
      ),
    );
  }
}
