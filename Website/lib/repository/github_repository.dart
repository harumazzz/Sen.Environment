import 'package:website/api/github_api.dart';
import 'package:website/model/github.dart';

class GithubRepository {
  final GithubApi _githubApi;

  const GithubRepository(this._githubApi);

  Future<GitHub> getRelease() async {
    return await _githubApi.getRelease();
  }
}
