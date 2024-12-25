import 'package:flutter/material.dart';
import 'package:website/application.dart';
import 'package:website/service_locator/service_locator.dart';
import 'package:firebase_core/firebase_core.dart';
import 'firebase_options.dart';

Future<void> main(
  List<String> arguments,
) async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );
  ServiceLocator.instance.registerSingleton();
  runApp(const Application());
}
