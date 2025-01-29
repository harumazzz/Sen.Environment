part of 'init_bloc.dart';

sealed class InitEvent extends Equatable {
  const InitEvent();

  @override
  List<Object?> get props => [];
}

final class SetStatusEvent extends InitEvent {
  const SetStatusEvent({required this.status});

  final InitailizeStatus status;

  @override
  List<Object?> get props => [status];
}

final class ShowSnackBarEvent extends InitEvent {
  const ShowSnackBarEvent({required this.text});

  final String text;

  @override
  List<Object?> get props => [text];
}

final class ShowAlertDialog extends InitEvent {
  const ShowAlertDialog({required this.type, required this.enable});

  final AlertDialogShowType type;

  final bool enable;

  @override
  List<Object?> get props => [type];
}

final class CreateTakeShootFunction extends InitEvent {
  const CreateTakeShootFunction({required this.takeShoot});

  final Future<Uint8List?> Function() takeShoot;

  @override
  List<Object?> get props => [takeShoot];
}
