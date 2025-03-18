part of 'init_bloc.dart';

enum InitailizeStatus { initailize, success }

enum AlertDialogShowType { clear, config, shortcut }

final class InitState extends Equatable {
  const InitState({
    required this.status,
    required this.alertDialogEnable,
    this.text,
    this.takeShoot,
  });
  factory InitState.initailize() {
    return InitState(
      status: InitailizeStatus.initailize,
      alertDialogEnable: HashMap.of({
        AlertDialogShowType.clear: false,
        AlertDialogShowType.config: false,
        AlertDialogShowType.shortcut: false,
      }),
    );
  }

  final InitailizeStatus status;

  final String? text;

  final HashMap<AlertDialogShowType, bool> alertDialogEnable;

  final Future<Uint8List?> Function()? takeShoot;

  InitState copyWith({
    InitailizeStatus? status,
    String? text,
    Future<Uint8List?> Function()? takeShoot,
    HashMap<AlertDialogShowType, bool>? alertDialogEnable,
  }) {
    final map = HashMap<AlertDialogShowType, bool>();
    for (final entry in (alertDialogEnable ?? this.alertDialogEnable).entries) {
      map[entry.key] = entry.value;
    }
    return InitState(
      status: status ?? this.status,
      takeShoot: takeShoot ?? this.takeShoot,
      text: text ?? this.text,
      alertDialogEnable: map,
    );
  }

  @override
  List<Object?> get props => [status, text, alertDialogEnable, takeShoot];
}
