import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:website/model/changelog.dart';

class ChangelogApi {
  final _ref = FirebaseFirestore.instance.collection('changelog');

  Future<List<Changelog>> getChangelog() async {
    final snapshot = await _ref.orderBy('date', descending: true).get();
    final List<Changelog> result = [];
    for (final child in snapshot.docs) {
      result.add(Changelog.fromJson(child.data()));
    }
    return result;
  }
}
