import 'package:cloud_firestore/cloud_firestore.dart';

import '../api/extension_api.dart';
import '../model/extension.dart';

class ExtensionRepository {
  ExtensionRepository(this._extensionApi);
  final ExtensionApi _extensionApi;

  DocumentSnapshot? _previousDocument;

  Future<List<Extension>> getExtensions({int limit = 10}) async {
    return await _extensionApi.getExtensions(
      limit: limit,
      previousDocument: _previousDocument,
      onFetch: (previousDocument) {
        _previousDocument = previousDocument;
      },
    );
  }

  bool get canFetch => _previousDocument != null;
}
