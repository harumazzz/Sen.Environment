import 'package:website/model/github.dart';
import 'package:dio/dio.dart' as dio;
import 'package:website/service_locator/service_locator.dart';

class GithubApi {
  const GithubApi();

  Future<GitHub> getRelease() async {
    final response = await ServiceLocator.instance.get<dio.Dio>().get(
      '/release',
    );
    if (response.statusCode == 200) {
      return GitHub.fromJson(response.data);
    } else {
      throw Exception(response.data.toString());
    }
  }
}
