part of 'ticker_bloc.dart';

sealed class TickerState extends Equatable {
  const TickerState(this.tick);
  final int tick;

  @override
  List<Object> get props => [tick];
}

final class TickerInitial extends TickerState {
  const TickerInitial(super.tick);

  @override
  String toString() => 'TickerInitial { tick: $tick }';
}

final class TickerRunPause extends TickerState {
  const TickerRunPause(super.tick);

  @override
  String toString() => 'TickerRunPause { tick: $tick }';
}

final class TickerRunInProgress extends TickerState {
  const TickerRunInProgress(super.tick);

  @override
  String toString() => 'TickerRunInProgress { tick: $tick }';
}

final class TickerRunComplete extends TickerState {
  const TickerRunComplete() : super(0);
}
