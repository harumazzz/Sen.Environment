part of 'message_bloc.dart';

@immutable
sealed class MessageState extends Equatable {
  const MessageState({required this.messages});
  final List<Message> messages;

  @override
  List<Object?> get props => [messages];

  Message operator [](int index) {
    return messages[index];
  }

  int get size => messages.length;

  bool get isEmpty => messages.isEmpty;

  Message get last => messages[messages.length - 1];

  Message lastWhere(bool Function(Message element) query) =>
      messages[messages.lastIndexWhere(query)];
}

final class MessageInitialState extends MessageState {
  const MessageInitialState({required super.messages});

  MessageInitialState copyWith({List<Message>? messages}) {
    return MessageInitialState(messages: messages ?? this.messages);
  }
}

final class MessageClearState extends MessageState {
  const MessageClearState({required super.messages});

  MessageClearState copyWith({List<Message>? messages}) {
    return MessageClearState(messages: messages ?? this.messages);
  }
}

final class MessageAddState extends MessageState {
  const MessageAddState({required super.messages});

  MessageAddState copyWith({List<Message>? messages}) {
    return MessageAddState(messages: messages ?? this.messages);
  }
}

final class MessageScrollState extends MessageState {
  const MessageScrollState({required super.messages});

  MessageScrollState copyWith({List<Message>? messages}) {
    return MessageScrollState(messages: messages ?? this.messages);
  }
}

final class MessageRegisterContextMenuState extends MessageState {
  const MessageRegisterContextMenuState({
    required super.messages,
    required this.options,
  });

  final List<SelectOption> options;

  MessageRegisterContextMenuState copyWith({
    List<Message>? messages,
    List<SelectOption>? options,
  }) {
    return MessageRegisterContextMenuState(
      messages: messages ?? this.messages,
      options: options ?? this.options,
    );
  }

  @override
  List<Object?> get props => [...super.props, options];
}
