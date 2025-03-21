import 'dart:convert';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import '../extension/context.dart';
import '../service/ui_helper.dart';

class JsonViewer extends StatefulWidget {
  const JsonViewer({super.key, required this.name, required this.message});
  final String name;
  final String message;

  @override
  State<JsonViewer> createState() => _JsonViewerState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('message', message));
    properties.add(StringProperty('name', name));
  }
}

class _JsonViewerState extends State<JsonViewer> {
  late ScrollController _verticalController;
  late ScrollController _horizontalController;

  @override
  void initState() {
    _verticalController = ScrollController();
    _horizontalController = ScrollController();
    super.initState();
  }

  @override
  void dispose() {
    _verticalController.dispose();
    _horizontalController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Card(
      margin: const EdgeInsets.all(12.0),
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12.0)),
      elevation: 4.0,
      child: Padding(
        padding: const EdgeInsets.all(12.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [_buildHeader(), _buildJsonViewer(), _buildActions()],
        ),
      ),
    );
  }

  Widget _buildHeader() {
    return Row(
      mainAxisAlignment: MainAxisAlignment.spaceBetween,
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Expanded(
          child: Text(
            widget.name,
            style: const TextStyle(fontWeight: FontWeight.bold, fontSize: 16.0),
            softWrap: true,
            overflow: TextOverflow.visible,
          ),
        ),
        IconButton(
          icon: const Icon(Symbols.copy_all, size: 20),
          tooltip: context.los.copy_json,
          onPressed: _copyToClipboard,
        ),
      ],
    );
  }

  Widget _buildJsonViewer() {
    return Expanded(
      child: ClipRRect(
        borderRadius: BorderRadius.circular(8.0),
        child: Container(
          width: double.infinity,
          padding: const EdgeInsets.all(12),
          decoration: BoxDecoration(
            color: const Color(0xFF252526),
            borderRadius: BorderRadius.circular(8),
          ),
          child: Scrollbar(
            controller: _verticalController,
            thumbVisibility: true,
            child: SingleChildScrollView(
              controller: _verticalController,
              child: SingleChildScrollView(
                controller: _horizontalController,
                scrollDirection: Axis.horizontal,
                child: SizedBox(
                  width: double.maxFinite,
                  child: RichText(
                    text: TextSpan(
                      style: const TextStyle(
                        fontFamily: 'GoogleSans',
                        fontSize: 16,
                        height: 1.4,
                        color: Colors.white,
                      ),
                      children: _highlightJson(),
                    ),
                  ),
                ),
              ),
            ),
          ),
        ),
      ),
    );
  }

  Widget _buildActions() {
    return Row(
      mainAxisAlignment: MainAxisAlignment.end,
      children: [...UIHelper.buildSimpleAction(context: context)],
    );
  }

  void _copyToClipboard() {
    Clipboard.setData(ClipboardData(text: widget.message));
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(
        content: Text(
          context.los.copied_to_clipboard,
          softWrap: true,
          overflow: TextOverflow.visible,
        ),
        width: UIHelper.ofDesktop(builder: () => 400.0),
        behavior: SnackBarBehavior.floating,
        duration: const Duration(milliseconds: 1500),
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(10.0),
        ),
      ),
    );
  }

  List<TextSpan> _highlightJson() {
    final json = const JsonEncoder.withIndent(
      '    ',
    ).convert(jsonDecode(widget.message));
    final List<TextSpan> spans = [];

    final RegExp regex = RegExp(
      r'(\s*)("(.*?)")\s*:\s*("[^"]*"|\d+|true|false|null)?|([{}[\],])',
      multiLine: true,
    );
    var lastIndex = 0;
    for (final match in regex.allMatches(json)) {
      if (match.start > lastIndex) {
        spans.add(TextSpan(text: json.substring(lastIndex, match.start)));
      }
      if (match.group(1) != null) {
        spans.add(TextSpan(text: match.group(1)));
      }
      if (match.group(2) != null) {
        spans.add(
          TextSpan(
            text: match.group(2),
            style: const TextStyle(color: Colors.green),
          ),
        );
        spans.add(
          const TextSpan(text: ': ', style: TextStyle(color: Colors.white70)),
        );
      }
      if (match.group(4) != null) {
        Color valueColor = Colors.blue;
        if (match.group(4)!.startsWith('"')) {
          valueColor = Colors.blue;
        } else if (match.group(4) == 'true' || match.group(4) == 'false') {
          valueColor = Colors.orange;
        } else if (match.group(4) == 'null') {
          valueColor = Colors.grey;
        } else {
          valueColor = Colors.cyan;
        }
        spans.add(
          TextSpan(text: match.group(4), style: TextStyle(color: valueColor)),
        );
      }
      if (match.group(5) != null) {
        spans.add(
          TextSpan(
            text: match.group(5),
            style: const TextStyle(color: Colors.white70),
          ),
        );
      }

      lastIndex = match.end;
    }
    if (lastIndex < json.length) {
      spans.add(TextSpan(text: json.substring(lastIndex)));
    }

    return spans;
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('name', widget.name));
    properties.add(StringProperty('message', widget.message));
  }
}
