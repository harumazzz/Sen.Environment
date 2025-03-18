import 'package:cloud_firestore/cloud_firestore.dart';
import '../model/changelog.dart';

class ChangelogApi {
  const ChangelogApi();

  Future<List<Changelog>> getChangelog() async {
    final ref = FirebaseFirestore.instance.collection('changelog');
    final snapshot = await ref.orderBy('date', descending: true).get();
    final List<Changelog> result = [];
    result.addAll(snapshot.docs.map((e) => Changelog.fromJson(e.data())));
    return result;
  }
}
