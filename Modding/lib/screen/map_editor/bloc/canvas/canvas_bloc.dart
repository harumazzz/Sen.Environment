import 'package:equatable/equatable.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../../../extension/platform.dart';
import '../../include/controller.dart';
import '../../models/map_const.dart';

part 'canvas_event.dart';
part 'canvas_state.dart';

class CanvasBloc extends Bloc<CanvasEvent, CanvasState> {
  CanvasBloc() : super(CanvasState.initialize()) {
    on<InitailizeState>(_initailize);
    on<InitCameraViewOffset>(_initCameraViewOffset);
    on<UpdateMarqueeEvent>(_updateMarquee);
    on<TransformControllerAddListener>(_addListener);
  }

  void _initailize(InitailizeState event, Emitter<CanvasState> emit) {
    emit(CanvasState.initialize());
  }

  void _initCameraViewOffset(
    InitCameraViewOffset event,
    Emitter<CanvasState> emit,
  ) {
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    final safeAdditionalWidth =
        isDesktopPlatform
            ? MapConst.safeDesktopAdditionalWidth
            : MapConst.safeMobileAdditionalWidth;
    final safeAdditionalHeight =
        isDesktopPlatform
            ? MapConst.safeDesktopAdditionalHeight
            : MapConst.safeMobileAdditionalHeight;
    final startPositionX = -(safeAdditionalWidth / 2) + 2;
    final startPositionY = -(safeAdditionalHeight / 2) + 2;
    state.canvasController.transformationController.value =
        Matrix4.identity()..translate(startPositionX, startPositionY);
    emit(state);
  }

  void _addListener(
    TransformControllerAddListener event,
    Emitter<CanvasState> emit,
  ) {
    final matrix =
        state.canvasController.transformationController.value.clone();
    final newState = CanvasState(
      canvasController: CanvasController(
        transformationController:
            TransformationController()..addListener(event.listener),
      ),
    );
    newState.canvasController.focusNode.requestFocus();
    newState.canvasController.transformationController.value = matrix;
    emit(newState);
  }

  void _updateMarquee(UpdateMarqueeEvent event, Emitter<CanvasState> emit) {
    final newState = CanvasState(
      canvasController: CanvasController(
        transformationController:
            state.canvasController.transformationController,
      ),
    );
    newState.canvasController.marqueeStart = event.start;
    newState.canvasController.marqueeEnd = event.end;
    emit(newState);
  }
}
