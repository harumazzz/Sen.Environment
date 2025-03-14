part of 'backup_setting_bloc.dart';

@immutable
sealed class BackupSettingState extends Equatable {
  final Map<String, dynamic>? configuration;
  const BackupSettingState({
    this.configuration,
  });
  @override
  List<Object?> get props => [];
}

class BackupSettingInitial extends BackupSettingState {
  const BackupSettingInitial({
    super.configuration,
  });
}

class ConfigurationLoading extends BackupSettingState {
  const ConfigurationLoading({
    super.configuration,
  });
}

class ConfigurationLoaded extends BackupSettingState {
  const ConfigurationLoaded({
    super.configuration,
  });
}

class ConfigurationApplied extends BackupSettingState {
  const ConfigurationApplied({
    super.configuration,
  });
}

class ConfigurationSaved extends BackupSettingState {
  const ConfigurationSaved({
    super.configuration,
  });
}

class ConfigurationUploaded extends BackupSettingState {
  const ConfigurationUploaded({
    super.configuration,
  });
}

class BackupError extends BackupSettingState {
  final String message;
  const BackupError({
    super.configuration,
    required this.message,
  });

  @override
  List<Object?> get props => [...super.props, message];
}
