part of 'javascript_cubit.dart';

class JavascriptState extends Equatable {
  final bool showConfirmDialog;

  final bool runAsLauncher;

  const JavascriptState({
    required this.showConfirmDialog,
    required this.runAsLauncher,
  });

  @override
  List<Object> get props => [showConfirmDialog, runAsLauncher];

  JavascriptState copyWith({
    bool? showConfirmDialog,
    bool? runAsLauncher,
  }) {
    return JavascriptState(
      showConfirmDialog: showConfirmDialog ?? this.showConfirmDialog,
      runAsLauncher: runAsLauncher ?? this.runAsLauncher,
    );
  }

  factory JavascriptState.initialize() {
    return const JavascriptState(
      showConfirmDialog: true,
      runAsLauncher: false,
    );
  }

  @override
  String toString() => 'JavascriptState(showConfirmDialog: $showConfirmDialog, runAsLauncher: $runAsLauncher)';
}
