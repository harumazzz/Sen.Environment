import 'package:flutter_dotenv/flutter_dotenv.dart';
import 'package:get_it/get_it.dart';
import 'package:website/api/changelog_api.dart';
import 'package:website/api/github_api.dart';
import 'package:website/repository/changelog_repository.dart';
import 'package:website/repository/github_repository.dart';
import 'package:dio/dio.dart' as dio;

class ServiceLocator {
  static final GetIt _getIt = GetIt.asNewInstance();

  static const ServiceLocator instance = ServiceLocator._();

  const ServiceLocator._();

  void registerIfNot<T extends Object>(T object) {
    if (!_getIt.isRegistered<T>()) {
      _getIt.registerSingleton(object);
    }
  }

  void registerSingleton() {
    registerIfNot<GithubRepository>(const GithubRepository(GithubApi()));
    registerIfNot<ChangelogRepository>(
      const ChangelogRepository(ChangelogApi()),
    );
    registerIfNot(
      dio.Dio(
        dio.BaseOptions(
          baseUrl: dotenv.get('WEBSITE'),
          connectTimeout: const Duration(seconds: 60),
          receiveTimeout: const Duration(seconds: 60),
        ),
      ),
    );
  }

  T get<T extends Object>() {
    assert(_getIt.isRegistered<T>(), '$T is not registered');
    return _getIt.get<T>();
  }

  Future<void> reset() async => await _getIt.reset();
}
