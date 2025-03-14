part of 'add_option_bloc.dart';

@immutable
sealed class AddOptionState extends Equatable {
  const AddOptionState();

  @override
  List<Object?> get props => <Object?>[];
}

final class AddOptionInitial extends AddOptionState {
  const AddOptionInitial();
}

final class ExportLogState extends AddOptionState {
  const ExportLogState({required this.value});
  final String value;

  @override
  List<Object?> get props => <Object?>[value];
}

final class NoneOptionState extends AddOptionState {
  const NoneOptionState();
}

final class CaptureState extends AddOptionState {
  const CaptureState({required this.data});
  final Uint8List data;

  @override
  List<Object?> get props => <Object?>[...super.props, data];
}
