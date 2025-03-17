import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import 'package:meta/meta.dart';
import '../../model/message.dart';
import '../../model/select_option.dart';

part 'message_event.dart';
part 'message_state.dart';

class MessageBloc extends Bloc<MessageEvent, MessageState> {
  MessageBloc() : super(const MessageInitialState(messages: [])) {
    on<AddMessage>(_addMessage);
    on<ClearMessage>(_clearMessage);
    on<RegisterContextMenu>(_registerContextMenu);
  }

  void _addMessage(AddMessage event, Emitter<MessageState> emit) {
    final updatedMessages = [...state.messages, event.message];
    emit(MessageAddState(messages: updatedMessages));
    emit(MessageScrollState(messages: updatedMessages));
  }

  void _clearMessage(ClearMessage event, Emitter<MessageState> emit) {
    emit(const MessageClearState(messages: []));
    emit(const MessageScrollState(messages: []));
  }

  void _registerContextMenu(
    RegisterContextMenu event,
    Emitter<MessageState> emit,
  ) {
    emit(
      MessageRegisterContextMenuState(
        messages: state.messages,
        options: event.options,
      ),
    );
  }
}
