import 'package:website/api/changelog_api.dart';
import 'package:website/model/changelog.dart';

class ChangelogRepository {
  final ChangelogApi _changelogApi;

  ChangelogRepository(this._changelogApi);

  Future<List<Changelog>> getChangelog() async {
    return await _changelogApi.getChangelog();
  }
}
