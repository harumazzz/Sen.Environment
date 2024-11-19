library;

part 'wave/regular_wave.dart';
part 'wave/low_tide.dart';
part 'wave/storm_event.dart';
part 'wave/parachute_rain.dart';
part 'wave/spider_rain.dart';
part 'wave/raiding_party.dart';
part 'wave/frost_wind.dart';
part 'wave/dino_wave.dart';
part 'wave/tidal_change.dart';
part 'wave/jam_wave.dart';
part 'wave/portal_wave.dart';
part 'wave/ground_spawn.dart';
part 'wave/necromancy.dart';
part 'wave/level_module.dart';

// TODO : Implement Grid Spawn
// TODO : Implement Conveyor

abstract class Wave {
  dynamic toJson(String aliasesName);
}
