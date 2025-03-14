import 'dart:convert';
import 'dart:typed_data';

import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';
import 'package:screenshot/screenshot.dart';
import '../../model/message.dart';
import '../message_bloc/message_bloc.dart';

part 'add_option_event.dart';
part 'add_option_state.dart';

class AddOptionBloc extends Bloc<AddOptionEvent, AddOptionState> {
  AddOptionBloc() : super(const AddOptionInitial()) {
    _screenshotController = ScreenshotController();
    on<ExportLogEvent>(_onExportLog);
    on<ClearOptionEvent>(_onClear);
    on<CaptureMessageEvent>(_onCapture);
  }
  late ScreenshotController _screenshotController;

  Future<void> _onCapture(
    CaptureMessageEvent event,
    Emitter<AddOptionState> emit,
  ) async {
    final Uint8List? value = await _screenshotController.capture(
      delay: const Duration(milliseconds: 300),
    );
    if (value != null) {
      emit(CaptureState(data: value));
    }
  }

  void _onExportLog(ExportLogEvent event, Emitter<AddOptionState> emit) {
    final List<Map<String, dynamic>> value = <Map<String, dynamic>>[
      ...event.messageBloc.state.messages.map((Message e) => e.toJson()),
    ];
    final String result = const JsonEncoder.withIndent('\t').convert(value);
    emit(ExportLogState(value: result));
  }

  void _onClear(ClearOptionEvent event, Emitter<AddOptionState> emit) {
    emit(const NoneOptionState());
  }

  ScreenshotController get screenshotController => _screenshotController;
}
