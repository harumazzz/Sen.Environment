part of 'history_bloc.dart';

final class HistoryState extends Equatable {
  const HistoryState({
    required this.captureManager,
    required this.scrollController,
    required this.index,
  });
  factory HistoryState.initailize() {
    return HistoryState(
      captureManager: CaptureManager<ActionService<ActionModelType>>(),
      scrollController: ScrollController(),
      index: -1,
    );
  }

  final int index;

  final CaptureManager<ActionService<ActionModelType>> captureManager;

  final ScrollController scrollController;

  HistoryState copyWith({
    CaptureManager<ActionService<ActionModelType>>? captureManager,
    ScrollController? scrollController,
    int? index,
  }) {
    return HistoryState(
      captureManager: captureManager ?? this.captureManager,
      scrollController: scrollController ?? this.scrollController,
      index: index ?? this.index,
    );
  }

  @override
  List<Object?> get props => [index, captureManager, scrollController];
}
