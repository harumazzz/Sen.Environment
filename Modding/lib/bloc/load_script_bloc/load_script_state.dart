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
  const LoadScriptFailed({required this.message});
  final String message;

  @override
  List<Object> get props => [message];
}

class LoadScriptLoaded extends LoadScriptState {
  const LoadScriptLoaded({
    required this.kernel,
    required this.script,
    required this.allData,
    required this.filteredData,
  });

  factory LoadScriptLoaded.fromJson(Map<String, dynamic> map) {
    return LoadScriptLoaded(
      kernel: map['kernel']?.toInt() ?? 0,
      script: map['script']?.toInt() ?? 0,
      allData: List<Script>.from(map['data']?.map((x) => Script.fromJson(x))),
      filteredData: List<Script>.from(
        map['data']?.map((x) => Script.fromJson(x)),
      ),
    );
  }
  final int kernel;
  final int script;
  final List<Script> allData;
  final List<Script> filteredData;

  @override
  List<Object> get props => [kernel, script, allData, filteredData];

  LoadScriptLoaded copyWith({
    int? kernel,
    int? script,
    List<Script>? allData,
    List<Script>? filteredData,
  }) {
    return LoadScriptLoaded(
      kernel: kernel ?? this.kernel,
      script: script ?? this.script,
      allData: allData ?? this.allData,
      filteredData: filteredData ?? this.filteredData,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'kernel': kernel,
      'script': script,
      'data': allData.map((x) => x.toJson()).toList(),
    };
  }
}

class Script extends Equatable {
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
  final String path;
  final String name;
  final String description;

  Map<String, dynamic> toJson() {
    return {'path': path, 'name': name, 'description': description};
  }

  Script copyWith({String? path, String? name, String? description}) {
    return Script(
      path: path ?? this.path,
      name: name ?? this.name,
      description: description ?? this.description,
    );
  }

  @override
  List<Object?> get props => [path, name, description];
}
