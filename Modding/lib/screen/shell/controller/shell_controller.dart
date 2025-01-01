import 'dart:ffi';
import 'dart:isolate';

import 'package:async/async.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:sen/model/api.dart';
import 'package:sen/model/message.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/shell/helper/kernel_helper.dart';

class ShellController {
  final void Function(void Function()) setState;
  final void Function()? onAfter;
  final void Function()? onFinish;
  final void Function()? onBefore;
  final List<Message> messages;

  ShellController({
    required this.onFinish,
    required this.setState,
    required this.onAfter,
    required this.messages,
    this.onBefore,
  });

  void _handleQueryCommand(List<dynamic> event) {
    if (event.isEmpty) return;
    var command = event.removeAt(0);
    if (command == null) return;

    switch (command) {
      case 'display':
        _handleDisplayCommand(event);
        break;
      case 'finish':
        _handleFinishCommand();
        break;
    }
  }

  void _handleDisplayCommand(List<dynamic> event) {
    if (event.isEmpty) return;

    switch (event.length) {
      case 1:
        _sendDirectMessage(event[0]);
        break;
      case 2:
        _sendMessageWithSubtitle(event[0], event[1]);
        break;
      case 3:
        _sendMessageWithSubtitleAndColor(event[0], event[1], event[2]);
        break;
    }
  }

  void _sendDirectMessage(String message) {
    onBefore?.call();
    setState(() {
      messages.add(Message(title: message));
    });
    onAfter?.call();
  }

  void _sendMessageWithSubtitle(String message, String subtitle) {
    onBefore?.call();
    setState(() {
      messages.add(Message(title: message, subtitle: subtitle));
    });
    onAfter?.call();
  }

  void _sendMessageWithSubtitleAndColor(
    String title,
    String subtitle,
    String color,
  ) {
    onBefore?.call();
    setState(() {
      messages.add(Message(title: title, subtitle: subtitle, color: color));
    });
    onAfter?.call();
  }

  void _handleFinishCommand() {
    onFinish?.call();
  }

  Future<void> run({
    required WidgetRef ref,
    required List<String> arguments,
    required Future<void> Function(Pointer<CStringView> destination) inputString,
    required Future<void> Function(Pointer<CStringView> destination, List<String> restStatement) inputEnumeration,
    required Future<void> Function(Pointer<CStringView> destination) inputBoolean,
  }) async {
    final SettingState setting = ref.read(settingProvider);
    final mainReceivePort = ReceivePort();
    final mainStreamQueue = StreamQueue<dynamic>(mainReceivePort);
    await _launchIsolate(mainReceivePort, ref);
    final subSendPort = await mainStreamQueue.next as SendPort;
    _sendInitialConfiguration(subSendPort, setting, arguments);
    await _processEvents(
      mainStreamQueue,
      inputString: inputString,
      inputEnumeration: inputEnumeration,
      inputBoolean: inputBoolean,
    );
    await mainStreamQueue.cancel();
  }

  Future<void> _launchIsolate(
    ReceivePort mainReceivePort,
    WidgetRef ref,
  ) async {
    final kernelPath = KernelHelper.queryKernelPath(ref);
    await Isolate.spawn(KernelHelper.run, [mainReceivePort.sendPort, kernelPath]);
  }

  void _sendInitialConfiguration(
    SendPort subSendPort,
    SettingState setting,
    List<String> arguments,
  ) {
    subSendPort.send(['${setting.toolChain}/Script/main.js', (setting.toolChain), arguments]);
  }

  Future<void> _processEvents(
    StreamQueue<dynamic> mainStreamQueue, {
    required Future<void> Function(Pointer<CStringView> destination) inputString,
    required Future<void> Function(Pointer<CStringView> destination, List<String> restStatement) inputEnumeration,
    required Future<void> Function(Pointer<CStringView> destination) inputBoolean,
  }) async {
    while (await mainStreamQueue.hasNext) {
      var mainEvent = await mainStreamQueue.next as List<dynamic>?;
      if (mainEvent == null) {
        mainEvent = await mainStreamQueue.next as List<dynamic>;
        break;
      }
      if (_isInputRequest(mainEvent)) {
        await _handleInputRequest(
          mainEvent,
          inputString: inputString,
          inputEnumeration: inputEnumeration,
          inputBoolean: inputBoolean,
        );
      } else {
        _handleQueryCommand(mainEvent);
      }
    }
  }

  bool _isInputRequest(List<dynamic> mainEvent) {
    return mainEvent[0] != null && (mainEvent[0] as String).contains('input');
  }

  Future<void> _handleInputRequest(
    List<dynamic> mainEvent, {
    required Future<void> Function(Pointer<CStringView> destination) inputString,
    required Future<void> Function(Pointer<CStringView> destination, List<String> restStatement) inputEnumeration,
    required Future<void> Function(Pointer<CStringView> destination) inputBoolean,
  }) async {
    final callbackState = Pointer<Bool>.fromAddress(mainEvent[1]);
    final destination = Pointer<CStringView>.fromAddress(mainEvent[2]);
    switch (mainEvent[0]) {
      case 'input_string':
        await inputString(destination);
        break;
      case 'input_enumeration':
        final restStatement = (mainEvent[3] as List<String>);
        await inputEnumeration(destination, restStatement);
        break;
      case 'input_boolean':
        await inputBoolean(destination);
        break;
    }
    callbackState.value = true;
  }
}
