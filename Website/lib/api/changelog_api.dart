import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:website/model/changelog.dart';

class ChangelogApi {
  const ChangelogApi();

  Future<List<Changelog>> getChangelog() async {
    final ref = FirebaseFirestore.instance.collection('changelog');
    final snapshot = await ref.orderBy('date', descending: true).get();
    final List<Changelog> result = [];
    for (final child in snapshot.docs) {
      result.add(Changelog.fromJson(child.data()));
    }
    return result;
  }
}
