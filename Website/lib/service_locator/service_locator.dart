import 'package:get_it/get_it.dart';
import 'package:website/api/changelog_api.dart';
import 'package:website/api/github_api.dart';
import 'package:website/repository/changelog_repository.dart';
import 'package:website/repository/github_repository.dart';

class ServiceLocator {
  final GetIt _getIt = GetIt.asNewInstance();
  static final ServiceLocator instance = ServiceLocator._();
  ServiceLocator._();

  void registerIfNot<T extends Object>(T object) {
    if (!_getIt.isRegistered<T>()) {
      _getIt.registerSingleton(object);
    }
  }

  void registerSingleton() {
    registerIfNot<GithubRepository>(GithubRepository(GithubApi()));
    registerIfNot<ChangelogRepository>(ChangelogRepository(ChangelogApi()));
  }

  T get<T extends Object>() {
    return _getIt.get<T>();
  }

  Future<void> reset() async => await _getIt.reset();
}
