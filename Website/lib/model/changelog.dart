import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:equatable/equatable.dart';

class Changelog extends Equatable {
  const Changelog({
    this.date,
    this.version,
    this.specialThanks,
    this.updateChanges,
  });

  factory Changelog.fromJson(Map<String, dynamic> json) {
    return Changelog(
      version: json['version'],
      date: (json['date'] as Timestamp).toDate(),
      specialThanks:
          json['special_thanks'] != null
              ? (json['special_thanks'] as List<dynamic>)
                  .map((e) => e.toString())
                  .toList()
              : null,
      updateChanges:
          json['update_changes'] != null
              ? (json['update_changes'] as List<dynamic>)
                  .map((e) => e.toString())
                  .toList()
              : null,
    );
  }
  final String? version;
  final DateTime? date;
  final List<String>? specialThanks;
  final List<String>? updateChanges;

  @override
  List<Object?> get props => [version, date, specialThanks, updateChanges];
}
