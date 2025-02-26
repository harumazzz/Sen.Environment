part of 'launch_status_bloc.dart';

sealed class LaunchStatusState extends Equatable {
  final bool isRunning;

  const LaunchStatusState({
    required this.isRunning,
  });

  @override
  List<Object> get props => [isRunning];
}

final class LaunchStatusInitial extends LaunchStatusState {
  const LaunchStatusInitial() : super(isRunning: false);
}

final class LaunchStatusStart extends LaunchStatusState {
  const LaunchStatusStart() : super(isRunning: true);
}

final class LaunchStatusLoading extends LaunchStatusState {
  const LaunchStatusLoading() : super(isRunning: true);
}

final class LaunchStatusMain extends LaunchStatusState {
  const LaunchStatusMain() : super(isRunning: true);
}

final class LaunchStatusEnd extends LaunchStatusState {
  const LaunchStatusEnd() : super(isRunning: false);
}
