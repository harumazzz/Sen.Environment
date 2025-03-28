part of 'backup_setting_bloc.dart';

@immutable
sealed class BackupSettingEvent extends Equatable {
  const BackupSettingEvent();

  @override
  List<Object> get props => [];
}

class LoadConfiguration extends BackupSettingEvent {
  const LoadConfiguration({required this.toolChain});
  final String toolChain;

  @override
  List<Object> get props => [toolChain];
}

class ApplyConfiguration extends BackupSettingEvent {
  const ApplyConfiguration({required this.toolChain});
  final String toolChain;
  @override
  List<Object> get props => [toolChain];
}

class UploadConfiguration extends BackupSettingEvent {
  const UploadConfiguration({
    required this.toolChain,
    required this.initialDirectoryBloc,
  });
  final String toolChain;
  final InitialDirectoryBloc initialDirectoryBloc;
  @override
  List<Object> get props => [toolChain, initialDirectoryBloc];
}

class SaveConfiguration extends BackupSettingEvent {
  const SaveConfiguration();

  @override
  List<Object> get props => [];
}
