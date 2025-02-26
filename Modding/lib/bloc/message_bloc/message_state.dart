part of 'message_bloc.dart';

@immutable
sealed class MessageState extends Equatable {
  final List<Message> messages;

  const MessageState({
    required this.messages,
  });

  @override
  List<Object?> get props => [messages];

  operator [](int index) {
    return messages[index];
  }

  int get size => messages.length;

  bool get isEmpty => messages.isEmpty;
}

final class MessageInitialState extends MessageState {
  const MessageInitialState({required super.messages});

  MessageInitialState copyWith({
    List<Message>? messages,
  }) {
    return MessageInitialState(
      messages: messages ?? this.messages,
    );
  }
}

final class MessageClearState extends MessageState {
  const MessageClearState({required super.messages});

  MessageClearState copyWith({
    List<Message>? messages,
  }) {
    return MessageClearState(
      messages: messages ?? this.messages,
    );
  }
}

final class MessageAddState extends MessageState {
  const MessageAddState({required super.messages});

  MessageAddState copyWith({
    List<Message>? messages,
  }) {
    return MessageAddState(
      messages: messages ?? this.messages,
    );
  }
}

final class MessageScrollState extends MessageState {
  const MessageScrollState({required super.messages});

  MessageScrollState copyWith({
    List<Message>? messages,
  }) {
    return MessageScrollState(
      messages: messages ?? this.messages,
    );
  }
}
