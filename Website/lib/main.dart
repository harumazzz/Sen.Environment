import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:website/application.dart';
import 'package:website/service_locator/service_locator.dart';
import 'package:firebase_core/firebase_core.dart';
import 'firebase_options.dart';
import 'dart:js_interop' as js;

@js.JS('flutterWebRendererReady')
external void flutterWebRendererReady();

Future<void> main(
  List<String> arguments,
) async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );
  ServiceLocator.instance.registerSingleton();
  if (kIsWeb) {
    Future.delayed(Duration(milliseconds: 500), () {
      flutterWebRendererReady();
    });
  }
  runApp(const Application());
}
