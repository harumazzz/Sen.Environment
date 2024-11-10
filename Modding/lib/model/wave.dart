library;

part 'wave/spawn.dart';
part 'wave/regular_wave.dart';
part 'wave/low_tide.dart';
part 'wave/storm_event.dart';

abstract class Wave {
  dynamic toJson(String aliasesName);
}
