import 'package:cloud_firestore/cloud_firestore.dart';

import '../model/extension.dart';

class ExtensionApi {
  const ExtensionApi();

  Future<List<Extension>> getExtensions({
    required int limit,
    required DocumentSnapshot? previousDocument,
    required Function(DocumentSnapshot? previousDocument) onFetch,
  }) async {
    final ref = FirebaseFirestore.instance.collection('extension');
    var query = ref.limit(limit);
    if (previousDocument != null) {
      query = query.startAfterDocument(previousDocument);
    }
    final snapshot = await query.get();
    if (snapshot.docs.isNotEmpty) {
      onFetch(snapshot.docs.last);
    } else {
      onFetch(null);
    }
    final result = <Extension>[];
    result.addAll(snapshot.docs.map((e) => Extension.fromJson(e.data())));
    return result;
  }
}
