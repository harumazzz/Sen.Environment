import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';
import 'package:sen/model/message.dart';

part 'message_event.dart';
part 'message_state.dart';

class MessageBloc extends Bloc<MessageEvent, MessageState> {
  MessageBloc() : super(const MessageInitialState(messages: [])) {
    on<AddMessage>(_addMessage);
    on<ClearMessage>(_clearMessage);
  }

  Future<void> _addMessage(
    AddMessage event,
    Emitter<MessageState> emit,
  ) async {
    await Future.delayed(const Duration(milliseconds: 50));
    final updatedMessages = [...state.messages, event.message];
    emit(MessageAddState(messages: updatedMessages));
    await Future.delayed(const Duration(milliseconds: 50));
    emit(MessageScrollState(messages: updatedMessages));
  }

  Future<void> _clearMessage(
    ClearMessage event,
    Emitter<MessageState> emit,
  ) async {
    emit(const MessageClearState(messages: []));
    await Future.delayed(const Duration(milliseconds: 40));
    emit(const MessageScrollState(messages: []));
  }
}
