import 'dart:ffi';
import 'dart:io';
import 'dart:isolate';

import 'package:ffi/ffi.dart';
import 'package:sen/model/api.dart';
import 'package:sen/model/build_distribution.dart';

class CommandHandler {
  final SendPort? sendPort;

  CommandHandler(this.sendPort);

  void handle(
    String command,
    List<String> result,
    Pointer<CStringView> destination,
  ) {
    switch (command) {
      case 'display':
        _handleDisplayMessageCommand(result);
        break;
      case 'input':
        _handleInputCommand(destination);
        break;
      case 'input_boolean':
        _handleInputBooleanCommand(destination);
        break;
      case 'input_enumeration':
        _handleInputEnumerationCommand(result, destination);
        break;
      case 'version':
        _handleVersionCommand(destination);
        break;
      case 'is_gui':
        _handleIsGuiCommand(destination);
        break;
      case 'pick_file':
        _handlePickFile(destination);
        break;
      case 'pick_directory':
        _handlePickDirectory(destination);
        break;
      case 'finish':
        _handleFinishCallback();
        break;
      default:
        _handleUnexpectedCallback(result);
        break;
    }
  }

  void _handleUnexpectedCallback(List<String> command) {
    sendPort?.send(['unexpected', ...command]);
  }

  void _handlePickFile(Pointer<CStringView> destination) {
    final state = calloc<Bool>();
    state.value = false;
    sendPort?.send(['pick_file', state.address, destination.address]);
    while (!state.value) {
      sleep(const Duration(milliseconds: 10));
    }

    calloc.free(state);
  }

  void _handlePickDirectory(Pointer<CStringView> destination) {
    final state = calloc<Bool>();
    state.value = false;
    sendPort?.send(['pick_directory', state.address, destination.address]);
    while (!state.value) {
      sleep(const Duration(milliseconds: 10));
    }

    calloc.free(state);
  }

  void _handleFinishCallback() {
    sendPort?.send(['finish']);
  }

  void _handleDisplayMessageCommand(List<String> result) {
    switch (result.length) {
      case 1:
        sendPort?.send(['display', result[0]]);
        break;
      case 2:
        sendPort?.send(['display', result[0], result[1]]);
        break;
      case 3:
        sendPort?.send(['display', result[0], result[1], result[2]]);
        break;
    }
  }

  void _handleInputCommand(Pointer<CStringView> destination) {
    final state = calloc<Bool>();
    state.value = false;
    sendPort?.send(['input_string', state.address, destination.address]);

    while (!state.value) {
      sleep(const Duration(milliseconds: 10));
    }

    calloc.free(state);
  }

  void _handleInputBooleanCommand(Pointer<CStringView> destination) {
    final state = calloc<Bool>();
    state.value = false;
    sendPort?.send(['input_boolean', state.address, destination.address]);

    while (!state.value) {
      sleep(const Duration(milliseconds: 10));
    }

    calloc.free(state);
  }

  void _handleInputEnumerationCommand(List<String> result, Pointer<CStringView> destination) {
    var state = calloc<Bool>();
    state.value = false;
    sendPort?.send([
      'input_enumeration',
      state.address,
      destination.address,
      [...result]
    ]);

    while (!state.value) {
      sleep(const Duration(milliseconds: 10));
    }

    calloc.free(state);
  }

  void _handleVersionCommand(
    Pointer<CStringView> destination,
  ) {
    final version = BuildDistribution.version.toString();
    final ptr = calloc<Uint8>(version.length + 1);
    final typedList = ptr.asTypedList(version.length + 1);
    typedList.setAll(0, version.codeUnits);
    typedList[version.length] = 0;
    destination.ref
      ..size = version.length
      ..value = ptr;

    sendPort?.send([null]);
  }

  void _handleIsGuiCommand(Pointer<CStringView> destination) {
    final isGui = 1.toString();
    final ptr = calloc<Uint8>(isGui.length + 1);
    final typedList = ptr.asTypedList(isGui.length + 1);
    typedList.setAll(0, isGui.codeUnits);
    typedList[isGui.length] = 0;
    destination.ref
      ..size = isGui.length
      ..value = ptr;
    sendPort?.send([null]);
  }
}
