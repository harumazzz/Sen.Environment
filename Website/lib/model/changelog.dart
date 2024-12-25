import 'package:cloud_firestore/cloud_firestore.dart';

class Changelog {
  String? version;
  DateTime? date;
  List<String>? specialThanks;
  List<String>? updateChanges;

  Changelog({
    this.date,
    this.version,
    this.specialThanks,
    this.updateChanges,
  });

  Changelog.fromJson(Map<String, dynamic> json) {
    version = json['version'];
    date = (json['date'] as Timestamp).toDate();
    if (json['special_thanks'] != null) {
      specialThanks = [...(json['special_thanks'] as List<dynamic>).map((e) => e.toString())];
    }
    if (json['update_changes'] != null) {
      updateChanges = [...(json['update_changes'] as List<dynamic>).map((e) => e.toString())];
    }
    version = json['version'];
  }
}
