import 'dart:async';
import 'dart:ffi';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:screenshot/screenshot.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/model/api.dart';
import 'package:sen/model/message.dart';
import 'package:sen/screen/shell/controller/client_interaction_controller.dart';
import 'package:sen/screen/shell/model/running_state.dart';
import 'package:sen/screen/shell/view/boolean_stage.dart';
import 'package:sen/screen/shell/view/client_view.dart';
import 'package:sen/screen/shell/view/drop_handler.dart';
import 'package:sen/screen/shell/view/enumeration_stage.dart';
import 'package:sen/screen/shell/view/exit_handler.dart';
import 'package:sen/screen/shell/view/finished_stage.dart';
import 'package:sen/screen/shell/view/option_selector.dart';
import 'package:sen/screen/shell/controller/shell_controller.dart';
import 'package:sen/screen/shell/view/string_stage.dart';
import 'package:sen/service/file_helper.dart';
import 'package:loading_animation_widget/loading_animation_widget.dart';
import 'package:sen/widget/hotkey.dart';

class ShellScreen extends StatefulWidget {
  final List<String> arguments;

  final bool launchImmediately;

  const ShellScreen({
    super.key,
    required this.arguments,
    required this.launchImmediately,
  });

  @override
  State<ShellScreen> createState() => _ShellScreenState();
}

class _ShellScreenState extends State<ShellScreen> {
  TextEditingController? _inputController;
  late ScrollController _scrollController;
  RunningState _state = RunningState.idle;
  late List<Message> _messages;
  String _stage = '';
  Completer<String?>? _completer;
  late ClientInteractionController _stateController;
  late List<String> _enumeration;
  late String _value;
  late ShellController _shellController;
  late ScreenshotController _screenshotController;

  @override
  void initState() {
    _inputController = TextEditingController();
    _scrollController = ScrollController();
    _screenshotController = ScreenshotController();
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
      inputController: _inputController,
    );
    super.initState();
    if (widget.launchImmediately) {
      _run(widget.arguments);
    }
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
      _state = RunningState.finished;
    });
    _scrollToBottom();
  }

  void _setPendingJob() {
    setState(() {
      _state = RunningState.running;
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

  Future<void> _pickFile(
    Pointer<CStringView> destination,
  ) async {
    return await _stateController.pickFile(destination);
  }

  Future<void> _pickDirectory(
    Pointer<CStringView> destination,
  ) async {
    return await _stateController.pickDirectory(destination);
  }

  void _run(
    List<String> arguments,
  ) async {
    _setPendingJob();
    _clearMessage();
    await _shellController.run(
      context: context,
      arguments: arguments,
      inputString: _inputString,
      inputEnumeration: _inputEnumeration,
      inputBoolean: _inputBoolean,
      pickFile: _pickFile,
      pickDirectory: _pickDirectory,
    );
    _setFinishState();
    return;
  }

  Widget _makeStage() {
    if (_state == RunningState.finished) {
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
          onUploadFile: () async {
            void setWorkingDirectory(String source) {
              BlocProvider.of<InitialDirectoryCubit>(context).setDirectoryOfFile(source: source);
            }

            final file = await FileHelper.uploadFile(
              initialDirectory: BlocProvider.of<InitialDirectoryCubit>(context).state.initialDirectory,
            );
            if (file != null) {
              setWorkingDirectory(file);
            }
            return file;
          },
          onUploadDirectory: () async {
            void setWorkingDirectory(String source) {
              BlocProvider.of<InitialDirectoryCubit>(context).setDirectoryOfDirectory(source: source);
            }

            final directory = await FileHelper.uploadDirectory(
              initialDirectory: BlocProvider.of<InitialDirectoryCubit>(context).state.initialDirectory,
            );
            if (directory != null) {
              setWorkingDirectory(directory);
            }
            return directory;
          },
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
    final los = AppLocalizations.of(context)!;
    _completer!.complete(value);
    if (value != null) {
      _messages.add(Message(title: '${los.argument_got}: ', subtitle: value, color: 'green'));
    }
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

  Future<void> _showDialog(
    Uint8List image,
  ) async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) {
        return AlertDialog(
          title: Text(los.screenshot_taken),
          content: Image.memory(
            image,
          ),
          actions: [
            TextButton(
              child: Text(los.save),
              onPressed: () async {
                void closeDialog() => Navigator.of(context).pop();
                var file = await FileHelper.saveFile(
                  initialDirectory: BlocProvider.of<InitialDirectoryCubit>(context).state.initialDirectory,
                  suggestedName: 'screenshot.png',
                );
                if (file != null) {
                  if (RegExp(r'\.png$', caseSensitive: false).hasMatch(file)) {
                    file += '.png';
                  }
                  FileHelper.writeBuffer(source: file, data: image);
                  closeDialog();
                }
              },
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
  }

  void _takeScreenshot() async {
    final imageBytes = await _screenshotController.capture();

    if (imageBytes != null) {
      await _showDialog(imageBytes);
    }
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return HotkeyBuilder(
      child: Screenshot(
        controller: _screenshotController,
        child: ExitHandler(
          finished: _state != RunningState.running,
          child: Scaffold(
            appBar: AppBar(
              title: Text(los.shell),
              actions: [
                Tooltip(
                  message: los.take_screenshot,
                  child: IconButton(
                    onPressed: _takeScreenshot,
                    icon: const Icon(Symbols.screenshot),
                  ),
                ),
              ],
            ),
            body: DropHandler(
              inputController: _inputController,
              child: ClientView(
                state: _state,
                messages: _messages,
                scrollController: _scrollController,
                makeStage: _makeStage,
                stage: _stage,
                onLaunch: () => _run(widget.arguments),
              ),
            ),
          ),
        ),
      ),
    );
  }
}
