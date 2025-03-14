import 'dart:async';
import 'dart:isolate';

import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';
import '../../../../model/worldmap.dart';
import '../stage/stage_bloc.dart';
import '../../../../service/file_helper.dart';
import 'package:path/path.dart' as p;

part 'autosave_event.dart';
part 'autosave_state.dart';

class AutosaveBloc extends Bloc<AutosaveEvent, AutosaveState> {
  AutosaveBloc({required this.stageBloc})
    : super(const AutosaveState(files: [])) {
    on<SaveEvent>(_onAutosaveEvent);
    on<CleanAutosaveEvent>(_onClean);
    _timer = Timer.periodic(const Duration(seconds: 15), (Timer t) {
      add(const SaveEvent());
    });
  }
  late final Timer _timer;

  final StageBloc stageBloc;

  Future<void> _onAutosaveEvent(
    SaveEvent event,
    Emitter<AutosaveState> emit,
  ) async {
    Future<void> saveCurrentState(List<dynamic> data) async {
      final stageBloc = data[0] as StageBloc;
      final destination = data[1] as String;
      final state = stageBloc.state;
      final worldMap = WorldMap(
        list: [
          WorldData(
            worldName: state.worldName,
            worldID: state.worldId,
            resGroupID: state.resGroupId,
            boundingRect: state.boundingRect,
            pieces: state.pieces.values.toList(),
            events: state.events.values.toList(),
            creationTime: DateTime.now().millisecondsSinceEpoch ~/ 1000,
          ),
        ],
      );
      await FileHelper.createDirectoryAsync(p.dirname(destination));
      await FileHelper.writeJsonAsync(
        source: destination,
        data: WorldMap.toJson(worldMap),
      );
    }

    if (stageBloc.state.worldName == 'none') {
      return;
    }
    final destination =
        '${await FileHelper.getWorkingDirectory()}/${DateTime.now().millisecond}';
    state.files.add(destination);
    await Isolate.spawn(saveCurrentState, [stageBloc, destination]);
  }

  Future<void> _onClean(
    CleanAutosaveEvent event,
    Emitter<AutosaveState> emit,
  ) async {
    Future<void> cleanCurrentState(List<dynamic> data) async {
      final files = data[0] as List<String>;
      for (final file in files) {
        await FileHelper.deleteFileAsync(source: file);
      }
    }

    final destination =
        '${await FileHelper.getWorkingDirectory()}/${DateTime.now().millisecond}';
    state.files.add(destination);
    await Isolate.spawn(cleanCurrentState, [state.files]);
  }

  @override
  Future<void> close() {
    _timer.cancel();
    return super.close();
  }
}
