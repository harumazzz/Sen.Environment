import 'package:equatable/equatable.dart';

typedef Transform = List<double>;

typedef Color = List<double>;

typedef Matrix = List<double>;

class FrameNode extends Equatable {
  const FrameNode({
    required this.index,
    required this.duration,
    required this.resource,
    required this.sprite,
    this.firstFrame,
    required this.transform,
    required this.color,
  });

  factory FrameNode.fromJson(Map<String, dynamic> json) {
    return FrameNode(
      index: json['index'],
      duration: json['duration'],
      resource: json['resource'],
      sprite: json['sprite'],
      firstFrame: json['first_frame'] != null ? (json['first_frame']) : null,
      transform: List<double>.from(json['transform']),
      color: List<double>.from(json['color']),
    );
  }
  final int index;
  final int duration;
  final int resource;
  final bool sprite;
  final int? firstFrame;
  final Transform transform;
  final Color color;

  Map<String, dynamic> toJson() {
    return {
      'index': index,
      'duration': duration,
      'resource': resource,
      'sprite': sprite,
      'firstFrame': firstFrame,
      'transform': transform,
      'color': color,
    };
  }

  FrameNode copyWith({
    int? index,
    int? duration,
    int? resource,
    bool? sprite,
    int? firstFrame,
    Transform? transform,
    Color? color,
  }) {
    return FrameNode(
      index: index ?? this.index,
      duration: duration ?? this.duration,
      resource: resource ?? this.resource,
      sprite: sprite ?? this.sprite,
      firstFrame: this.firstFrame,
      transform: transform ?? this.transform,
      color: color ?? this.color,
    );
  }

  @override
  List<Object?> get props {
    return [index, duration, resource, sprite, firstFrame, transform, color];
  }
}

typedef FrameNodeList = List<FrameNode>;

typedef FrameNodeStructure = Map<int, FrameNodeList>;

enum State { stateNull, stateFalse, stateTrue }

class Model extends Equatable {
  const Model({
    required this.state,
    required this.resource,
    required this.sprite,
    required this.transform,
    required this.color,
    required this.frameStart,
    required this.frameDuration,
    this.index,
  });

  factory Model.fromJson(Map<String, dynamic> json) {
    return Model(
      state: State.values[json['state']],
      resource: json['resource'],
      sprite: json['sprite'],
      transform: List<double>.from(json['transform']),
      color: List<double>.from(json['color']),
      frameStart: json['frame_start'],
      frameDuration: json['frame_duration'],
      index: json['index'] != null ? (json['index']) : null,
    );
  }
  final State state;
  final int resource;
  final bool sprite;
  final Transform transform;
  final Color color;
  final int frameStart;
  final int frameDuration;
  final int? index;

  Model copyWith({
    State? state,
    int? resource,
    bool? sprite,
    Transform? transform,
    Color? color,
    int? frameStart,
    int? frameDuration,
    int? index,
  }) {
    return Model(
      state: state ?? this.state,
      resource: resource ?? this.resource,
      sprite: sprite ?? this.sprite,
      transform: transform ?? this.transform,
      color: color ?? this.color,
      frameStart: frameStart ?? this.frameStart,
      frameDuration: frameDuration ?? this.frameDuration,
      index: this.index,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'state': state,
      'resource': resource,
      'sprite': sprite,
      'transform': transform,
      'color': color,
      'frameStart': frameStart,
      'frameDuration': frameDuration,
      'index': index,
    };
  }

  @override
  List<Object?> get props {
    return [
      state,
      resource,
      sprite,
      transform,
      color,
      frameStart,
      frameDuration,
      index,
    ];
  }
}

class SexyAnimation extends Equatable {
  const SexyAnimation({
    required this.version,
    required this.frameRate,
    required this.position,
    required this.size,
    required this.image,
    required this.sprite,
    required this.mainSprite,
  });

  factory SexyAnimation.fromJson(Map<String, dynamic> json) {
    return SexyAnimation(
      version: json['version'],
      frameRate: json['frame_rate'],
      position: AnimationPosition.fromJson(json['position']),
      size: AnimationSize.fromJson(json['size']),
      image:
          (json['image'] as List)
              .map((i) => AnimationImage.fromJson(i))
              .toList(),
      sprite:
          (json['sprite'] as List)
              .map((i) => AnimationSprite.fromJson(i))
              .toList(),
      mainSprite: AnimationSprite.fromJson(json['main_sprite']),
    );
  }
  final int version;
  final int frameRate;
  final AnimationPosition position;
  final AnimationSize size;
  final List<AnimationImage> image;
  final List<AnimationSprite> sprite;
  final AnimationSprite mainSprite;

  SexyAnimation copyWith({
    int? version,
    int? frameRate,
    AnimationPosition? position,
    AnimationSize? size,
    List<AnimationImage>? image,
    List<AnimationSprite>? sprite,
    AnimationSprite? mainSprite,
  }) {
    return SexyAnimation(
      version: version ?? this.version,
      frameRate: frameRate ?? this.frameRate,
      position: position ?? this.position,
      size: size ?? this.size,
      image: image ?? this.image,
      sprite: sprite ?? this.sprite,
      mainSprite: mainSprite ?? this.mainSprite,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'version': version,
      'frameRate': frameRate,
      'position': position.toJson(),
      'size': size.toJson(),
      'image': image.map((x) => x.toJson()).toList(),
      'sprite': sprite.map((x) => x.toJson()).toList(),
      'mainSprite': mainSprite.toJson(),
    };
  }

  @override
  List<Object> get props {
    return [version, frameRate, position, size, image, sprite, mainSprite];
  }
}

class AnimationPosition extends Equatable {
  const AnimationPosition({required this.x, required this.y});

  factory AnimationPosition.fromJson(Map<String, dynamic> json) {
    return AnimationPosition(x: json['x'].toDouble(), y: json['y'].toDouble());
  }
  final double x;
  final double y;

  Map<String, dynamic> toJson() {
    return {'x': x, 'y': y};
  }

  AnimationPosition copyWith({double? x, double? y}) {
    return AnimationPosition(x: x ?? this.x, y: y ?? this.y);
  }

  @override
  List<Object> get props => [x, y];
}

class AnimationSize extends Equatable {
  const AnimationSize({required this.width, required this.height});

  factory AnimationSize.fromJson(Map<String, dynamic> json) {
    return AnimationSize(
      width: json['width'].toDouble(),
      height: json['height'].toDouble(),
    );
  }

  factory AnimationSize.fromMap(Map<String, dynamic> map) {
    return AnimationSize(
      width: map['width']?.toDouble() ?? 0.0,
      height: map['height']?.toDouble() ?? 0.0,
    );
  }
  final double width;
  final double height;

  AnimationSize copyWith({double? width, double? height}) {
    return AnimationSize(
      width: width ?? this.width,
      height: height ?? this.height,
    );
  }

  Map<String, dynamic> toJson() {
    return {'width': width, 'height': height};
  }

  @override
  List<Object> get props => [width, height];
}

class AnimationImage extends Equatable {
  const AnimationImage({
    required this.path,
    required this.id,
    required this.dimension,
    required this.transform,
  });

  factory AnimationImage.fromJson(Map<String, dynamic> json) {
    return AnimationImage(
      path: json['path'],
      id: json['id'],
      dimension: AnimationDimension.fromJson(json['dimension']),
      transform: List<double>.from(json['transform']),
    );
  }
  final String path;
  final String id;
  final AnimationDimension dimension;
  final List<double> transform;

  AnimationImage copyWith({
    String? path,
    String? id,
    AnimationDimension? dimension,
    List<double>? transform,
  }) {
    return AnimationImage(
      path: path ?? this.path,
      id: id ?? this.id,
      dimension: dimension ?? this.dimension,
      transform: transform ?? this.transform,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'path': path,
      'id': id,
      'dimension': dimension.toJson(),
      'transform': transform,
    };
  }

  @override
  List<Object> get props => [path, id, dimension, transform];
}

class AnimationDimension extends Equatable {
  const AnimationDimension({required this.width, required this.height});

  factory AnimationDimension.fromJson(Map<String, dynamic> json) {
    return AnimationDimension(width: json['width'], height: json['height']);
  }
  final int width;
  final int height;

  AnimationDimension copyWith({int? width, int? height}) {
    return AnimationDimension(
      width: width ?? this.width,
      height: height ?? this.height,
    );
  }

  Map<String, dynamic> toJson() {
    return {'width': width, 'height': height};
  }

  @override
  List<Object> get props => [width, height];
}

class AnimationSprite extends Equatable {
  const AnimationSprite({
    required this.name,
    required this.workArea,
    required this.frame,
  });

  factory AnimationSprite.fromJson(Map<String, dynamic> json) {
    return AnimationSprite(
      name: json['name'],
      workArea: AnimationWorkArea.fromJson(json['work_area']),
      frame:
          (json['frame'] as List)
              .map((i) => AnimationFrame.fromJson(i))
              .toList(),
    );
  }
  final String name;
  final AnimationWorkArea workArea;
  final List<AnimationFrame> frame;

  AnimationSprite copyWith({
    String? name,
    AnimationWorkArea? workArea,
    List<AnimationFrame>? frame,
  }) {
    return AnimationSprite(
      name: name ?? this.name,
      workArea: workArea ?? this.workArea,
      frame: frame ?? this.frame,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'name': name,
      'workArea': workArea.toJson(),
      'frame': frame.map((x) => x.toJson()).toList(),
    };
  }

  @override
  List<Object> get props => [name, workArea, frame];
}

class AnimationWorkArea extends Equatable {
  const AnimationWorkArea({required this.start, required this.duration});

  factory AnimationWorkArea.fromJson(Map<String, dynamic> json) {
    return AnimationWorkArea(start: json['start'], duration: json['duration']);
  }
  final int start;
  final int duration;

  AnimationWorkArea copyWith({int? start, int? duration}) {
    return AnimationWorkArea(
      start: start ?? this.start,
      duration: duration ?? this.duration,
    );
  }

  Map<String, dynamic> toJson() {
    return {'start': start, 'duration': duration};
  }

  @override
  List<Object> get props => [start, duration];
}

class AnimationFrame extends Equatable {
  const AnimationFrame({
    required this.label,
    required this.stop,
    required this.command,
    required this.remove,
    required this.append,
    required this.change,
  });

  factory AnimationFrame.fromJson(Map<String, dynamic> json) {
    return AnimationFrame(
      label: json['label'],
      stop: json['stop'],
      command:
          (json['command'] as List)
              .map((i) => AnimationCommand.fromJson(i))
              .toList(),
      remove:
          (json['remove'] as List).map((i) => int.parse(i.toString())).toList(),
      append:
          (json['append'] as List)
              .map((i) => AnimationAppend.fromJson(i))
              .toList(),
      change:
          (json['change'] as List)
              .map((i) => AnimationMove.fromJson(i))
              .toList(),
    );
  }
  final String label;
  final bool stop;
  final List<AnimationCommand> command;
  final List<int> remove;
  final List<AnimationAppend> append;
  final List<AnimationMove> change;

  AnimationFrame copyWith({
    String? label,
    bool? stop,
    List<AnimationCommand>? command,
    List<int>? remove,
    List<AnimationAppend>? append,
    List<AnimationMove>? change,
  }) {
    return AnimationFrame(
      label: label ?? this.label,
      stop: stop ?? this.stop,
      command: command ?? this.command,
      remove: remove ?? this.remove,
      append: append ?? this.append,
      change: change ?? this.change,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'label': label,
      'stop': stop,
      'command': command.map((x) => x.toJson()).toList(),
      'remove': remove,
      'append': append.map((x) => x.toJson()).toList(),
      'change': change.map((x) => x.toJson()).toList(),
    };
  }

  @override
  List<Object> get props {
    return [label, stop, command, remove, append, change];
  }
}

class AnimationCommand extends Equatable {
  const AnimationCommand({required this.command, required this.argument});

  factory AnimationCommand.fromJson(Map<String, dynamic> json) {
    return AnimationCommand(
      command: json['command'],
      argument: json['argument'],
    );
  }
  final String command;
  final String argument;

  AnimationCommand copyWith({String? command, String? argument}) {
    return AnimationCommand(
      command: command ?? this.command,
      argument: argument ?? this.argument,
    );
  }

  Map<String, dynamic> toJson() {
    return {'command': command, 'argument': argument};
  }

  @override
  List<Object> get props => [command, argument];
}

class AnimationAppend extends Equatable {
  const AnimationAppend({
    required this.index,
    required this.name,
    required this.resource,
    required this.sprite,
    required this.additive,
    required this.preloadFrame,
    required this.timeScale,
  });

  factory AnimationAppend.fromJson(Map<String, dynamic> json) {
    return AnimationAppend(
      index: json['index'],
      name: json['name'],
      resource: json['resource'],
      sprite: json['sprite'],
      additive: json['additive'],
      preloadFrame: json['preload_frame'],
      timeScale: json['time_scale'].toDouble(),
    );
  }
  final int index;
  final String name;
  final int resource;
  final bool sprite;
  final bool additive;
  final int preloadFrame;
  final double timeScale;

  Map<String, dynamic> toJson() {
    return {
      'index': index,
      'name': name,
      'resource': resource,
      'sprite': sprite,
      'additive': additive,
      'preloadFrame': preloadFrame,
      'timeScale': timeScale,
    };
  }

  AnimationAppend copyWith({
    int? index,
    String? name,
    int? resource,
    bool? sprite,
    bool? additive,
    int? preloadFrame,
    double? timeScale,
  }) {
    return AnimationAppend(
      index: index ?? this.index,
      name: name ?? this.name,
      resource: resource ?? this.resource,
      sprite: sprite ?? this.sprite,
      additive: additive ?? this.additive,
      preloadFrame: preloadFrame ?? this.preloadFrame,
      timeScale: timeScale ?? this.timeScale,
    );
  }

  @override
  List<Object> get props {
    return [index, name, resource, sprite, additive, preloadFrame, timeScale];
  }
}

class AnimationMove extends Equatable {
  const AnimationMove({
    required this.index,
    required this.transform,
    this.color,
    this.sourceRectangle,
    required this.spriteFrameNumber,
  });

  factory AnimationMove.fromJson(Map<String, dynamic> json) {
    return AnimationMove(
      index: json['index'],
      transform: List<double>.from(json['transform']),
      color: json['color'] != null ? List<double>.from(json['color']) : null,
      sourceRectangle:
          json['source_rectangle'] != null
              ? List<double>.from(json['source_rectangle'])
              : null,
      spriteFrameNumber: json['sprite_frame_number'],
    );
  }
  final int index;
  final List<double> transform;
  final Color? color;
  final List<double>? sourceRectangle;
  final int spriteFrameNumber;

  AnimationMove copyWith({
    int? index,
    List<double>? transform,
    Color? color,
    List<double>? sourceRectangle,
    int? spriteFrameNumber,
  }) {
    return AnimationMove(
      index: index ?? this.index,
      transform: transform ?? this.transform,
      color: this.color,
      sourceRectangle: this.sourceRectangle,
      spriteFrameNumber: spriteFrameNumber ?? this.spriteFrameNumber,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'index': index,
      'transform': transform,
      'color': color,
      'sourceRectangle': sourceRectangle,
      'spriteFrameNumber': spriteFrameNumber,
    };
  }

  @override
  List<Object?> get props {
    return [index, transform, color, sourceRectangle, spriteFrameNumber];
  }
}
