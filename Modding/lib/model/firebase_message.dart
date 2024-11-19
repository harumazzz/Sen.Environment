import 'package:cloud_firestore/cloud_firestore.dart';

class FirebaseMessage {
  final String title;
  final String description;
  final DateTime createdAt;

  FirebaseMessage({
    required this.title,
    required this.description,
    required this.createdAt,
  });

  factory FirebaseMessage.fromMap(Map<String, dynamic> data) {
    return FirebaseMessage(
      title: data['title'] as String,
      description: data['description'] as String,
      createdAt: (data['createdAt'] as Timestamp).toDate(),
    );
  }

  Map<String, dynamic> toMap() {
    return {
      'title': title,
      'description': description,
      'createdAt': Timestamp.fromDate(createdAt),
    };
  }
}
