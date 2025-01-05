import 'package:flutter/material.dart';
import 'package:url_launcher/url_launcher.dart';

@immutable
class Hyperlink extends StatelessWidget {
  const Hyperlink({
    super.key,
    required this.title,
    required this.link,
  });

  final String title;

  final String link;

  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Text(
        title,
        style: const TextStyle(
          color: Colors.blue,
        ),
      ),
      onTap: () async {
        final uri = Uri.parse(link);
        await launchUrl(uri);
      },
    );
  }
}
