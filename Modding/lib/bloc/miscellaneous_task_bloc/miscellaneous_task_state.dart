part of 'miscellaneous_task_bloc.dart';

@immutable
sealed class MiscellaneousTaskState extends Equatable {
  const MiscellaneousTaskState();

  @override
  List<Object?> get props => [];
}

final class MiscellaneousTaskInitial extends MiscellaneousTaskState {
  const MiscellaneousTaskInitial();
}

class SettingsInitial extends MiscellaneousTaskState {
  const SettingsInitial();
}

class BackupInProgress extends MiscellaneousTaskState {
  const BackupInProgress();
}

class BackupCompleted extends MiscellaneousTaskState {
  const BackupCompleted();
}

class DownloadingScript extends MiscellaneousTaskState {
  const DownloadingScript();
}

class ScriptDownloaded extends MiscellaneousTaskState {
  const ScriptDownloaded();
}

class ScriptDownloadFailed extends MiscellaneousTaskState {
  const ScriptDownloadFailed({required this.error});
  final String error;

  @override
  List<Object> get props => [error];
}
