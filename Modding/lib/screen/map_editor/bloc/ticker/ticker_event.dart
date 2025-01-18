part of 'ticker_bloc.dart';

sealed class TickerEvent {
  const TickerEvent();
}

final class TickerStarted extends TickerEvent {
  const TickerStarted();
}

final class TickerPaused extends TickerEvent {
  const TickerPaused();
}

final class TickerResumed extends TickerEvent {
  const TickerResumed();
}

class TickerReset extends TickerEvent {
  const TickerReset();
}

class _TickerTicked extends TickerEvent {
  const _TickerTicked({required this.tick});
  final int tick;
}
