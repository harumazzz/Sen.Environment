abstract class Client {
  Future<void> start();

  Future<void> finish();

  Future<List<String>> callback(List<String> arguments);
}
