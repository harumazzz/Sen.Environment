import '../api/github_api.dart';
import '../model/github.dart';

class GithubRepository {
  const GithubRepository(this._githubApi);
  final GithubApi _githubApi;

  Future<GitHub> getRelease() async {
    return await _githubApi.getRelease();
  }
}
