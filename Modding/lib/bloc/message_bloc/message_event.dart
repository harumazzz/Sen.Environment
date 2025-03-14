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
