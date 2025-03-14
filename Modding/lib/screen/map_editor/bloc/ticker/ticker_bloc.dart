import 'dart:async';

import 'package:bloc/bloc.dart';
import 'package:equatable/equatable.dart';
import '../../include/ticker.dart';

part 'ticker_event.dart';
part 'ticker_state.dart';

class TickerBloc extends Bloc<TickerEvent, TickerState> {
  TickerBloc({
    required Ticker ticker,
    required int tickStart,
    required int tickEnd,
    int tickDuration = 1000, // 1000 milis = 1s
    bool repeat = true,
  }) : _ticker = ticker,
       _tickStart = tickStart,
       _tickEnd = tickEnd,
       _tickDuration = tickDuration,
       _repeat = repeat,
       super(TickerInitial(tickStart)) {
    on<TickerStarted>(_onStarted);
    on<TickerPaused>(_onPaused);
    on<TickerResumed>(_onResumed);
    on<TickerReset>(_onReset);
    on<_TickerTicked>(_onTicked);
    _onPlayed();
  }

  final bool _repeat;

  final Ticker _ticker;

  final int _tickDuration;

  final int _tickStart;

  final int _tickEnd;

  //int get _tickCount => _tickEnd  - _tickStart;

  StreamSubscription<int>? _tickerSubscription;

  @override
  Future<void> close() {
    _tickerSubscription?.cancel();
    return super.close();
  }

  void _onPlayed() {
    _tickerSubscription?.cancel();
    _tickerSubscription = _ticker
        .tick(tickStart: _tickStart, tickEnd: _tickEnd, duration: _tickDuration)
        .listen((tick) => add(_TickerTicked(tick: tick)));
  }

  void _onStarted(TickerStarted event, Emitter<TickerState> emit) {
    emit(TickerRunInProgress(_tickStart));
    _onPlayed();
  }

  void _onPaused(TickerPaused event, Emitter<TickerState> emit) {
    if (state is TickerRunInProgress) {
      _tickerSubscription?.pause();
      emit(TickerRunPause(state.tick));
    }
  }

  void _onResumed(TickerResumed resume, Emitter<TickerState> emit) {
    if (state is TickerRunPause) {
      _tickerSubscription?.resume();
      emit(TickerRunInProgress(state.tick));
    }
  }

  void _onReset(TickerReset event, Emitter<TickerState> emit) {
    _tickerSubscription?.cancel();
    emit(TickerInitial(_tickStart));
  }

  void _onTicked(_TickerTicked event, Emitter<TickerState> emit) {
    if (event.tick < _tickEnd) {
      emit(TickerRunInProgress(event.tick));
    } else {
      if (_repeat) {
        emit(TickerRunInProgress(_tickStart));
        _onPlayed();
      } else {
        emit(const TickerRunComplete());
      }
    }
  }
}
