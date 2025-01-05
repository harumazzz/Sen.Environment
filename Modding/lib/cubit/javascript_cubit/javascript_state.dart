part of 'javascript_cubit.dart';

class JavascriptState extends Equatable {
  final bool showConfirmDialog;

  const JavascriptState({
    required this.showConfirmDialog,
  });

  @override
  List<Object> get props => [showConfirmDialog];

  JavascriptState copyWith({
    bool? showConfirmDialog,
  }) {
    return JavascriptState(
      showConfirmDialog: showConfirmDialog ?? this.showConfirmDialog,
    );
  }

  factory JavascriptState.initialize() {
    return const JavascriptState(showConfirmDialog: true);
  }

  @override
  String toString() => 'JavascriptState(showConfirmDialog: $showConfirmDialog)';
}
