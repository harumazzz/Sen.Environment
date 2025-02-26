part of 'launch_status_bloc.dart';

sealed class LaunchStatusEvent extends Equatable {
  const LaunchStatusEvent();

  @override
  List<Object> get props => [];
}

final class LaunchStatusBegin extends LaunchStatusEvent {
  final Client client;

  final List<String> arguments;

  const LaunchStatusBegin({
    required this.client,
    required this.arguments,
  });

  @override
  List<Object> get props => [client, arguments];
}

final class LaunchStatusComplete extends LaunchStatusEvent {
  const LaunchStatusComplete();
}

final class LaunchStatusSleep extends LaunchStatusEvent {
  const LaunchStatusSleep();
}
