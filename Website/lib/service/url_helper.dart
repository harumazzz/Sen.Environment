import 'package:url_launcher_web/url_launcher_web.dart';

class UrlHelper {
  const UrlHelper._();

  static Future<void> launch({required String link}) async {
    final plugin = UrlLauncherPlugin();
    if (!await plugin.launch(link)) {
      throw Exception('Could not launch $link');
    }
  }
}
