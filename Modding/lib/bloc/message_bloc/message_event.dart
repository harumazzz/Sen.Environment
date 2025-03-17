part of 'message_bloc.dart';

@immutable
sealed class MessageEvent extends Equatable {
  const MessageEvent();

  @override
  List<Object?> get props => [];
}

final class AddMessage extends MessageEvent {
  const AddMessage({required this.message});
  final Message message;
}

final class ClearMessage extends MessageEvent {
  const ClearMessage();
}

final class RegisterContextMenu extends MessageEvent {
  const RegisterContextMenu({required this.options});

  final List<SelectOption> options;

  @override
  List<Object?> get props => [options];
}
