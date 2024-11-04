abstract class Wave {}

class Spawn {
  final int row;
  final String typename;

  Spawn({
    required this.row,
    required this.typename,
  });
}

class RegularWave implements Wave {
  final List<Spawn> zombies;

  RegularWave({
    required this.zombies,
  });
}
