import 'package:web/web.dart' as web;

class DownloadHelper {
  const DownloadHelper._();

  static void downloadFile(String url) {
    final anchor = web.document.createElement('a') as web.HTMLAnchorElement;
    anchor.href = url;
    anchor.download = url;
    anchor.click();
  }
}
