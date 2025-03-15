import '../api/changelog_api.dart';
import '../model/changelog.dart';

class ChangelogRepository {
  const ChangelogRepository(this._changelogApi);
  final ChangelogApi _changelogApi;

  Future<List<Changelog>> getChangelog() async {
    return await _changelogApi.getChangelog();
  }
}
