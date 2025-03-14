part of 'launch_status_bloc.dart';

sealed class LaunchStatusEvent extends Equatable {
  const LaunchStatusEvent();

  @override
  List<Object> get props => [];
}

final class LaunchStatusBegin extends LaunchStatusEvent {
  const LaunchStatusBegin({
    required this.client,
    required this.setting,
    required this.arguments,
  });
  final Client client;

  final SettingsCubit setting;

  final List<String> arguments;

  @override
  List<Object> get props => [client, setting, arguments];
}

final class LaunchStatusComplete extends LaunchStatusEvent {
  const LaunchStatusComplete();
}

final class LaunchStatusSleep extends LaunchStatusEvent {
  const LaunchStatusSleep();
}
