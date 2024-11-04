class Level {
  int? version;
  List<dynamic>? objects;

  Level({this.version, this.objects});

  Level.fromJson(Map<String, dynamic> json) {
    version = json['version'];
    if (json['objects'] != null) {
      objects = [];
      json['objects'].forEach((v) {
        objects!.add(v);
      });
    }
  }

  Map<String, dynamic> toJson() {
    final Map<String, dynamic> data = <String, dynamic>{};
    data['version'] = version;
    if (objects != null) {
      data['objects'] = objects;
    }
    return data;
  }
}
