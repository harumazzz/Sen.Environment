part of 'load_script_bloc.dart';

@immutable
abstract class LoadScriptState extends Equatable {
  const LoadScriptState();

  @override
  List<Object> get props => [];
}

class LoadScriptInitial extends LoadScriptState {}

class LoadScriptLoading extends LoadScriptState {}

class LoadScriptFailed extends LoadScriptState {
  final String message;

  const LoadScriptFailed({
    required this.message,
  });

  @override
  List<Object> get props => [message];
}

class LoadScriptLoaded extends LoadScriptState {
  final int kernel;
  final int script;
  final List<Script> data;

  const LoadScriptLoaded({
    required this.kernel,
    required this.script,
    required this.data,
  });

  @override
  List<Object> get props => [kernel, script, data];

  LoadScriptLoaded copyWith({
    int? kernel,
    int? script,
    List<Script>? data,
  }) {
    return LoadScriptLoaded(
      kernel: kernel ?? this.kernel,
      script: script ?? this.script,
      data: data ?? this.data,
    );
  }

  @override
  String toString() => 'LoadScriptLoaded(kernel: $kernel, script: $script, data: $data)';

  Map<String, dynamic> toJson() {
    return {
      'kernel': kernel,
      'script': script,
      'data': data.map((x) => x.toJson()).toList(),
    };
  }

  factory LoadScriptLoaded.fromJson(Map<String, dynamic> map) {
    return LoadScriptLoaded(
      kernel: map['kernel']?.toInt() ?? 0,
      script: map['script']?.toInt() ?? 0,
      data: List<Script>.from(map['data']?.map((x) => Script.fromJson(x))),
    );
  }
}

class Script extends Equatable {
  final String path;
  final String name;
  final String description;

  const Script({
    required this.path,
    required this.name,
    required this.description,
  });

  factory Script.fromJson(Map<String, dynamic> json) {
    return Script(
      path: json['path'],
      name: json['name'],
      description: json['description'],
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'path': path,
      'name': name,
      'description': description,
    };
  }

  Script copyWith({
    String? path,
    String? name,
    String? description,
  }) {
    return Script(
      path: path ?? this.path,
      name: name ?? this.name,
      description: description ?? this.description,
    );
  }

  @override
  List<Object?> get props => [path, name, description];
}
