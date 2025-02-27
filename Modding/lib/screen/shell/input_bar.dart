import 'package:desktop_drop/desktop_drop.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/bloc/add_option_bloc/add_option_bloc.dart';
import 'package:sen/bloc/interaction_bloc/interaction_bloc.dart';
import 'package:sen/bloc/message_bloc/message_bloc.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/extension/context.dart';
import 'package:sen/service/file_helper.dart';
import 'package:sen/service/ui_helper.dart';

class InputBar extends StatefulWidget {
  const InputBar({super.key});

  @override
  State<InputBar> createState() => _InputBarState();
}

class _InputBarState extends State<InputBar> {
  late TextEditingController _controller;

  late FocusNode _focusNode;

  late final GlobalKey<PopupMenuButtonState<String>> _popupMenuKey;

  @override
  void initState() {
    _popupMenuKey = GlobalKey<PopupMenuButtonState<String>>();
    _controller = TextEditingController();
    _focusNode = FocusNode();
    _focusNode.requestFocus();
    super.initState();
  }

  @override
  void dispose() {
    _controller.dispose();
    _focusNode.dispose();
    super.dispose();
  }

  void _onDisplayOption() {
    _popupMenuKey.currentState?.showButtonMenu();
  }

  void _onSend() {
    _focusNode.unfocus();
    context.read<InteractionBloc>().add(StringInputCompleteEvent(value: _controller.text));
  }

  Future<void> _onAttach(
    String value,
  ) async {
    var file = null as String?;
    final initialDirectory = BlocProvider.of<InitialDirectoryCubit>(context).state.initialDirectory;
    switch (value) {
      case 'pick_file':
        file = await FileHelper.uploadFile(initialDirectory: initialDirectory);
        break;
      case 'pick_directory':
        file = await FileHelper.uploadDirectory(initialDirectory: initialDirectory);
        break;
      case 'save_file':
        file = await FileHelper.saveFile(initialDirectory: initialDirectory);
        break;
    }
    if (file != null) {
      _controller.text = file;
    }
  }

  Widget _buildTextField() {
    return Expanded(
      child: Card(
        surfaceTintColor: Theme.of(context).colorScheme.surface,
        elevation: 4.0,
        child: Padding(
          padding: const EdgeInsets.symmetric(horizontal: 8.0),
          child: TextField(
            textAlign: TextAlign.start,
            focusNode: _focusNode,
            controller: _controller,
            decoration: InputDecoration(
              border: InputBorder.none,
              hintText: '${context.los.input_value}...',
              suffixIcon: PopupMenuButton<String>(
                initialValue: '',
                elevation: 4.0,
                icon: const Icon(
                  Symbols.attach_file,
                  size: 24.0,
                ),
                tooltip: context.los.attach,
                onSelected: _onAttach,
                itemBuilder: (BuildContext context) => <PopupMenuEntry<String>>[
                  PopupMenuItem<String>(
                    value: 'pick_file',
                    child: Text(context.los.upload_file),
                  ),
                  PopupMenuItem<String>(
                    value: 'pick_directory',
                    child: Text(context.los.upload_directory),
                  ),
                  PopupMenuItem<String>(
                    value: 'save_file',
                    child: Text(context.los.save_file),
                  ),
                ],
              ),
              contentPadding: const EdgeInsets.only(
                left: 8.0,
                top: 12.0,
                bottom: 12.0,
              ),
            ),
          ),
        ),
      ),
    );
  }

  void _onAdd(String value) {
    switch (value) {
      case 'export_log':
        context.read<AddOptionBloc>().add(ExportLogEvent(messageBloc: context.read<MessageBloc>()));
        break;
      case 'screen_shot':
        context.read<AddOptionBloc>().add(const CaptureMessageEvent());
        break;
    }
  }

  @override
  Widget build(BuildContext context) {
    return BlocListener<AddOptionBloc, AddOptionState>(
      listener: (context, state) async {
        if (state is CaptureState) {
          await UIHelper.showScreenshotDialog(context, state.data);
        }
      },
      child: DropTarget(
        enable: true,
        onDragDone: (details) {
          if (details.files.isNotEmpty) {
            if (details.files.isNotEmpty) {
              var file = details.files.first;
              _controller.text = file.path;
            }
          }
        },
        child: Row(
          mainAxisAlignment: MainAxisAlignment.start,
          spacing: 10.0,
          children: [
            Container(
              height: 46.0,
              width: 46.0,
              margin: const EdgeInsets.only(left: 8.0),
              child: PopupMenuButton<String>(
                key: _popupMenuKey,
                tooltip: context.los.add,
                itemBuilder: (context) => [
                  PopupMenuItem(
                    value: 'export_log',
                    child: Text(context.los.export_log),
                  ),
                  PopupMenuItem(
                    value: 'screen_shot',
                    child: Text(context.los.take_screenshot),
                  ),
                ],
                onSelected: _onAdd,
                child: FloatingActionButton(
                  heroTag: 'display-btn',
                  onPressed: _onDisplayOption,
                  child: const Icon(Symbols.add),
                ),
              ),
            ),
            _buildTextField(),
            Container(
              height: 46.0,
              width: 46.0,
              margin: const EdgeInsets.only(right: 8.0),
              child: Tooltip(
                message: context.los.submit,
                child: FloatingActionButton(
                  heroTag: 'send-btn',
                  elevation: 4.0,
                  onPressed: _onSend,
                  child: const Icon(Symbols.send),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
