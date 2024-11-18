library;

part 'wave/spawn.dart';
part 'wave/regular_wave.dart';
part 'wave/low_tide.dart';
part 'wave/storm_event.dart';
part 'wave/parachute_rain.dart';

// TODO : Implement Spider Rain
// TODO : Implement Raiding Party
// TODO : Implement Ground Spawn
// TODO : Implement Necromancy
// TODO : Implement Grid Spawn
// TODO : Implement Portal Spawn
// TODO : Implement Frost Wind
// TODO : Implement Dino
// TODO : Implement Tide
// TODO : Implement Jam
// TODO : Implement Conveyor

abstract class Wave {
  dynamic toJson(String aliasesName);
}
