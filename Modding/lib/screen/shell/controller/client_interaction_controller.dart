import 'dart:async';
import 'dart:ffi';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/model/api.dart';
import 'package:sen/model/message.dart';
import 'package:sen/service/file_helper.dart';
import 'package:sen/service/pointer_helper.dart';

class ClientInteractionController {
  final BuildContext context;
  final ScrollController scrollController;
  final TextEditingController? inputController;
  final List<Message> messages;

  ClientInteractionController({
    required this.context,
    required this.scrollController,
    required this.inputController,
  }) : messages = [];

  void _scrollToBottom() {
    Future.delayed(const Duration(milliseconds: 100), () {
      scrollController.jumpTo(scrollController.position.maxScrollExtent);
    });
  }

  Future<void> _handleInput(
    Pointer<CStringView> destination,
    Future<String?> Function() fetchData,
  ) async {
    final completer = Completer<String?>();
    completer.complete(await fetchData());
    final e = await completer.future;
    if (e == null) return;

    final units = PointerHelper.toUint8List(e);
    final utf8Str = PointerHelper.utf8ListToCString(units);
    destination.ref
      ..size = units.length
      ..value = utf8Str;

    _scrollToBottom();
    await WidgetsBinding.instance.endOfFrame;
  }

  Future<void> inputString(
    Pointer<CStringView> destination,
    void Function(Completer<String?> completer) onSend,
  ) async {
    return await _handleInput(destination, () async {
      final completer = Completer<String?>();
      onSend(completer);
      return completer.future;
    });
  }

  Future<void> inputEnumeration(
    Pointer<CStringView> destination,
    List<String> restStatement,
    void Function(Completer<String?> completer, String value) onSend,
  ) async {
    return await _handleInput(destination, () async {
      final completer = Completer<String?>();
      final value = restStatement[0];
      onSend(completer, value);
      return completer.future;
    });
  }

  Future<void> inputBoolean(
    Pointer<CStringView> destination,
    void Function(Completer<String?> completer) onSend,
  ) async {
    return await _handleInput(destination, () async {
      final completer = Completer<String?>();
      onSend(completer);
      return completer.future;
    });
  }

  Future<void> pickFile(
    Pointer<CStringView> destination,
  ) async {
    return await _handleInput(destination, () async {
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
    });
  }

  Future<void> pickDirectory(
    Pointer<CStringView> destination,
  ) async {
    return await _handleInput(destination, () async {
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
    });
  }
}
