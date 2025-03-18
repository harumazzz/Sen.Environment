import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'application.dart';
import 'service/service_locator.dart';
import 'package:firebase_core/firebase_core.dart';
import 'firebase_options.dart';
import 'package:flutter_dotenv/flutter_dotenv.dart';
import 'package:flutter_web_plugins/url_strategy.dart' as url_strategy;
import 'package:flutter/services.dart';
import 'bridge/proxy.dart';

Future<void> main(List<String> arguments) async {
  WidgetsFlutterBinding.ensureInitialized();
  await dotenv.load(fileName: 'assets/environment');
  await SystemChrome.setEnabledSystemUIMode(SystemUiMode.edgeToEdge);
  url_strategy.usePathUrlStrategy();
  await Firebase.initializeApp(options: DefaultFirebaseOptions.currentPlatform);
  ServiceLocator.instance.registerSingleton();
  if (kIsWeb) {
    Future.delayed(
      const Duration(milliseconds: 600),
      () => flutterWebRendererReady(),
    );
  }
  runApp(const Application());
}
