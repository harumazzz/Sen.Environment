part of 'backup_setting_bloc.dart';

@immutable
sealed class BackupSettingState extends Equatable {
  const BackupSettingState({this.configuration});
  final Map<String, dynamic>? configuration;
  @override
  List<Object?> get props => [];
}

class BackupSettingInitial extends BackupSettingState {
  const BackupSettingInitial({super.configuration});
}

class ConfigurationLoading extends BackupSettingState {
  const ConfigurationLoading({super.configuration});
}

class ConfigurationLoaded extends BackupSettingState {
  const ConfigurationLoaded({super.configuration});
}

class ConfigurationApplied extends BackupSettingState {
  const ConfigurationApplied({super.configuration});
}

class ConfigurationSaved extends BackupSettingState {
  const ConfigurationSaved({super.configuration});
}

class ConfigurationUploaded extends BackupSettingState {
  const ConfigurationUploaded({super.configuration});
}

class BackupError extends BackupSettingState {
  const BackupError({super.configuration, required this.message});
  final String message;

  @override
  List<Object?> get props => [...super.props, message];
}
