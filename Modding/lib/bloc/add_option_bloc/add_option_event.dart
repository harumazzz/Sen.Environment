part of 'add_option_bloc.dart';

@immutable
sealed class AddOptionEvent extends Equatable {
  const AddOptionEvent();
  @override
  List<Object?> get props => [];
}

final class ExportLogEvent extends AddOptionEvent {
  final MessageBloc messageBloc;

  const ExportLogEvent({
    required this.messageBloc,
  });
}

final class ClearOptionEvent extends AddOptionEvent {
  const ClearOptionEvent();
}

final class CaptureMessageEvent extends AddOptionEvent {
  const CaptureMessageEvent();
}
