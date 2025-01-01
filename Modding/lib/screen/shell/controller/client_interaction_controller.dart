import 'dart:async';
import 'dart:ffi';

import 'package:flutter/material.dart';
import 'package:sen/model/api.dart';
import 'package:sen/model/message.dart';
import 'package:sen/service/pointer_service.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class ClientInteractionController {
  final BuildContext context;
  final ScrollController scrollController;
  final List<Message> messages;

  ClientInteractionController({
    required this.context,
    required this.scrollController,
  }) : messages = [];

  void _scrollToBottom() {
    Future.delayed(const Duration(milliseconds: 100), () {
      scrollController.jumpTo(scrollController.position.maxScrollExtent);
    });
  }

  Future<void> inputString(
    Pointer<CStringView> destination,
    void Function(Completer<String?> completer) onSend,
  ) async {
    final los = AppLocalizations.of(context)!;
    final completer = Completer<String?>();
    onSend(completer);
    final e = await completer.future;
    final providedValue = e!.isNotEmpty ? e : 'none';
    messages.add(
      Message(
        title: '${los.user_provided}:',
        subtitle: providedValue,
        color: 'default',
      ),
    );
    final units = PointerService.toUint8List(e);
    final utf8Str = PointerService.utf8ListToCString(units);
    destination.ref
      ..size = units.length
      ..value = utf8Str;

    _scrollToBottom();
    await WidgetsBinding.instance.endOfFrame;
  }

  Future<void> inputEnumeration(
    Pointer<CStringView> destination,
    List<String> restStatement,
    void Function(Completer<String?> completer, String value) onSend,
  ) async {
    final los = AppLocalizations.of(context)!;
    final completer = Completer<String?>();
    final value = restStatement[0];
    onSend(completer, value);
    final e = await completer.future;
    messages.add(
      Message(
        title: los.user_provided,
        subtitle: e,
        color: 'default',
      ),
    );
    final units = PointerService.toUint8List(e!);
    final utf8Str = PointerService.utf8ListToCString(units);
    destination.ref
      ..size = units.length
      ..value = utf8Str;

    _scrollToBottom();
    await WidgetsBinding.instance.endOfFrame;
  }

  Future<void> inputBoolean(
    Pointer<CStringView> destination,
    void Function(Completer<String?> completer) onSend,
  ) async {
    final los = AppLocalizations.of(context)!;
    final completer = Completer<String?>();
    onSend(completer);
    final e = await completer.future;
    messages.add(
      Message(
        title: los.user_provided,
        subtitle: e,
        color: 'default',
      ),
    );
    final units = PointerService.toUint8List(e!);
    final utf8Str = PointerService.utf8ListToCString(units);
    destination.ref
      ..size = units.length
      ..value = utf8Str;

    _scrollToBottom();
    await WidgetsBinding.instance.endOfFrame;
  }
}
