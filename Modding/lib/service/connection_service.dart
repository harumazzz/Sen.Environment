import 'package:connectivity_plus/connectivity_plus.dart';

class ConnectionService {
  static List<ConnectivityResult>? _connectivityResult;

  static Future<void> initialize() async {
    _connectivityResult = await (Connectivity().checkConnectivity());
  }

  static bool hasWifi() {
    return _connectivityResult!.contains(ConnectivityResult.wifi);
  }

  static bool hasMobileHotspot() {
    return _connectivityResult!.contains(ConnectivityResult.mobile);
  }

  static bool hasEthernet() {
    return _connectivityResult!.contains(ConnectivityResult.ethernet);
  }

  static bool hasInternet() {
    return hasWifi() || hasMobileHotspot() || hasEthernet();
  }
}
