import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:sen/model/firebase_message.dart';

class MessageRepository {
  final FirebaseFirestore _firestore;

  MessageRepository() : _firestore = FirebaseFirestore.instance;

  Stream<List<FirebaseMessage>> getMessage() {
    return _firestore.collection('message').snapshots().map(
          (snapshot) => snapshot.docs
              .map(
                (e) => FirebaseMessage.fromMap(e.data()),
              )
              .toList(),
        );
  }
}
