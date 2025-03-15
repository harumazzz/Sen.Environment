import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'application.dart';
import 'service_locator/service_locator.dart';
import 'package:firebase_core/firebase_core.dart';
import 'firebase_options.dart';
import 'dart:js_interop' as js;
import 'package:flutter_dotenv/flutter_dotenv.dart';
import 'package:flutter_web_plugins/url_strategy.dart' as url_strategy;

@js.JS('flutterWebRendererReady')
external void flutterWebRendererReady();

Future<void> main(List<String> arguments) async {
  WidgetsFlutterBinding.ensureInitialized();
  await dotenv.load(fileName: 'assets/environment');
  url_strategy.usePathUrlStrategy();
  await Firebase.initializeApp(options: DefaultFirebaseOptions.currentPlatform);
  ServiceLocator.instance.registerSingleton();
  if (kIsWeb) {
    Future.delayed(
      const Duration(milliseconds: 500),
      () => flutterWebRendererReady(),
    );
  }
  runApp(const Application());
}
