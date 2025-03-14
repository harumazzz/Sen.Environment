part of 'backup_setting_bloc.dart';

@immutable
sealed class BackupSettingEvent extends Equatable {
  const BackupSettingEvent();

  @override
  List<Object> get props => [];
}

class LoadConfiguration extends BackupSettingEvent {
  final String toolChain;
  const LoadConfiguration({
    required this.toolChain,
  });

  @override
  List<Object> get props => [toolChain];
}

class ApplyConfiguration extends BackupSettingEvent {
  final String toolChain;
  const ApplyConfiguration({
    required this.toolChain,
  });
  @override
  List<Object> get props => [
        toolChain,
      ];
}

class UploadConfiguration extends BackupSettingEvent {
  final String toolChain;
  final InitialDirectoryCubit initialDirectoryCubit;
  const UploadConfiguration({
    required this.toolChain,
    required this.initialDirectoryCubit,
  });
  @override
  List<Object> get props => [toolChain, initialDirectoryCubit];
}

class SaveConfiguration extends BackupSettingEvent {
  const SaveConfiguration();

  @override
  List<Object> get props => [];
}
