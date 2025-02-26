abstract class Client {
  void start();

  void finish();

  Future<List<String>> callback(
    List<String> arguments,
  );
}
