part of 'init_bloc.dart';

enum InitailizeStatus { initailize, success }

enum AlertDialogShowType { clear, config, shortcut }

final class InitState extends Equatable {
  const InitState(
      {required this.status,
      required this.alertDialogEnable,
      //   required this.screenshotController,
      this.text,
      this.takeShoot});

  final InitailizeStatus status;

  final String? text;

  final HashMap<AlertDialogShowType, bool> alertDialogEnable;

  // final ScreenshotController screenshotController;

  final Future<Uint8List?> Function()? takeShoot;

  factory InitState.initailize() {
    return InitState(
        status: InitailizeStatus.initailize,
        //    screenshotController: ScreenshotController(),
        alertDialogEnable: HashMap.from({
          AlertDialogShowType.clear: false,
          AlertDialogShowType.config: false,
          AlertDialogShowType.shortcut: false
        }));
  }

  InitState copyWith(
      {InitailizeStatus? status,
      String? text,
      //  ScreenshotController? screenshotController,
      Future<Uint8List?> Function()? takeShoot,
      HashMap<AlertDialogShowType, bool>? alertDialogEnable}) {
    final map = HashMap<AlertDialogShowType, bool>();
    for (final entry in (alertDialogEnable ?? this.alertDialogEnable).entries) {
      map[entry.key] = entry.value;
    }
    return InitState(
        status: status ?? this.status,
        // screenshotController: screenshotController ?? this.screenshotController,
        takeShoot: takeShoot ?? this.takeShoot,
        text: text ?? this.text,
        alertDialogEnable: map);
  }

  @override
  List<Object?> get props => [status, text, alertDialogEnable, takeShoot];
}
