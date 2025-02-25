import 'package:web/web.dart' as web;

class DownloadHelper {
  static void downloadFile(String url) {
    final anchor = web.document.createElement('a') as web.HTMLAnchorElement;
    anchor.href = url;
    anchor.download = url;
    anchor.click();
  }
}
