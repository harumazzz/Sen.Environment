import 'dart:convert';

import 'package:website/model/github.dart';
import 'package:http/http.dart' as http;

class GithubApi {
  final _url = 'https://api.github.com/repos/Haruma-VN/Sen.Environment/releases/tags/release';

  Future<GitHub> getRelease() async {
    final uri = Uri.parse(_url);
    final response = await http.get(uri).timeout(const Duration(seconds: 10));
    if (response.statusCode == 200) {
      return GitHub.fromJson(jsonDecode(response.body));
    } else {
      throw Exception(response.body.toString());
    }
  }
}
