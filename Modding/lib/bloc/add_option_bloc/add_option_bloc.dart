import 'dart:convert';

import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';
import 'package:screenshot/screenshot.dart';
import 'package:sen/bloc/message_bloc/message_bloc.dart';

part 'add_option_event.dart';
part 'add_option_state.dart';

class AddOptionBloc extends Bloc<AddOptionEvent, AddOptionState> {
  late ScreenshotController controller;

  AddOptionBloc() : super(const AddOptionInitial()) {
    controller = ScreenshotController();
    on<ExportLogEvent>(_onExportLog);
    on<ClearOptionEvent>(_onClear);
  }

  void _onExportLog(
    ExportLogEvent event,
    Emitter<AddOptionState> emit,
  ) {
    final value = [...event.messageBloc.state.messages.map((e) => e.toJson())];
    final result = const JsonEncoder.withIndent('\t').convert(value);
    emit(ExportLogState(value: result));
  }

  void _onClear(
    ClearOptionEvent event,
    Emitter<AddOptionState> emit,
  ) {
    emit(const NoneOptionState());
  }
}
