import 'package:flutter/material.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';

class OptionSelector extends StatelessWidget {
  final Future<String?> Function() onUploadFile;
  final Future<String?> Function() onUploadDirectory;
  final TextEditingController? inputController;
  final VoidCallback scrollToBottom;

  const OptionSelector({
    super.key,
    required this.onUploadFile,
    required this.onUploadDirectory,
    required this.inputController,
    required this.scrollToBottom,
  });

  Future<void> _handleUploadFile(VoidCallback pop) async {
    var result = await onUploadFile();
    if (result != null && inputController != null) {
      inputController!.text = result;
      scrollToBottom();
    }
    pop();
  }

  Future<void> _handleUploadDirectory(VoidCallback pop) async {
    var result = await onUploadDirectory();
    if (result != null && inputController != null) {
      inputController!.text = result;
      scrollToBottom();
    }
    pop();
  }

  void _handleBatchFunction(VoidCallback pop) {
    if (inputController != null) {
      inputController!.text = ':b';
    }
    pop();
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    pop() => Navigator.pop(context);
    return Wrap(
      children: [
        ListTile(
          leading: const Icon(Icons.upload_file),
          title: Text(los.upload_file),
          onTap: () => _handleUploadFile(pop),
        ),
        ListTile(
          leading: const Icon(Icons.folder),
          title: Text(los.upload_directory),
          onTap: () => _handleUploadDirectory(pop),
        ),
        ListTile(
          leading: const Icon(Icons.code),
          title: Text(los.batch_function),
          onTap: () => _handleBatchFunction(pop),
        ),
        ListTile(
          leading: const Icon(Icons.cancel),
          title: Text(los.cancel),
          onTap: () => Navigator.pop(context),
        ),
      ],
    );
  }
}
