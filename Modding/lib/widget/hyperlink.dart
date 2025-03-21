import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:url_launcher/url_launcher.dart';

import '../extension/context.dart';

@immutable
class Hyperlink extends StatefulWidget {
  const Hyperlink({super.key, required this.title, required this.link});

  final String title;

  final String link;

  @override
  State<Hyperlink> createState() => _HyperlinkState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('link', link));
    properties.add(StringProperty('title', title));
  }
}

class _HyperlinkState extends State<Hyperlink> {
  late bool _isHovered;

  @override
  void initState() {
    _isHovered = false;
    super.initState();
  }

  Future<void> _launchUrl() async {
    final uri = Uri.parse(widget.link);
    await launchUrl(uri, mode: LaunchMode.externalApplication);
  }

  Future<void> _showContextMenu(TapDownDetails details) async {
    final overlay = Overlay.of(context).context.findRenderObject() as RenderBox;
    await showMenu(
      context: context,
      position: RelativeRect.fromRect(
        details.globalPosition & const Size(40, 40),
        Offset.zero & overlay.size,
      ),
      items: [
        PopupMenuItem(
          onTap: () async {
            await _launchUrl();
            ContextMenuController.removeAny();
          },
          child: Text(context.los.play),
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return MouseRegion(
      onEnter: (_) => setState(() => _isHovered = true),
      onExit: (_) => setState(() => _isHovered = false),
      child: GestureDetector(
        onTap: _launchUrl,
        onSecondaryTapDown: _showContextMenu,
        child: Text(
          widget.title,
          style: TextStyle(
            color: Colors.blue.shade700,
            decoration:
                _isHovered ? TextDecoration.underline : TextDecoration.none,
            fontWeight: FontWeight.w500,
          ),
        ),
      ),
    );
  }
}
