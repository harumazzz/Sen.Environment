class Ticker {
  const Ticker();
  Stream<int> tick({
    required int tickStart,
    required int tickEnd,
    required int duration,
  }) {
    return Stream.periodic(
      Duration(milliseconds: duration),
      (x) => tickStart + x + 1,
    ).take(tickEnd - tickStart);
  }
}
