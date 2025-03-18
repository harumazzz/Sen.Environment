import 'package:desktop_drop/desktop_drop.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../bloc/add_option_bloc/add_option_bloc.dart';
import '../../bloc/interaction_bloc/interaction_bloc.dart';
import '../../bloc/message_bloc/message_bloc.dart';
import '../../cubit/initial_directory_cubit/initial_directory_cubit.dart';
import '../../extension/context.dart';
import '../../service/file_helper.dart';
import '../../service/ui_helper.dart';

class InputBar extends StatefulWidget {
  const InputBar({super.key});

  @override
  State<InputBar> createState() => _InputBarState();
}

class _InputBarState extends State<InputBar> {
  late TextEditingController _controller;

  late FocusNode _focusNode;

  late FocusNode _keyboardNode;

  late final GlobalKey<PopupMenuButtonState<String>> _popupMenuKey;

  @override
  void initState() {
    _popupMenuKey = GlobalKey<PopupMenuButtonState<String>>();
    _controller = TextEditingController();
    _focusNode = FocusNode();
    _keyboardNode = FocusNode();
    _focusNode.requestFocus();
    super.initState();
  }

  @override
  void dispose() {
    _controller.dispose();
    _focusNode.dispose();
    _keyboardNode.dispose();
    super.dispose();
  }

  void _onDisplayOption() {
    _popupMenuKey.currentState?.showButtonMenu();
  }

  void _onSend() {
    _focusNode.unfocus();
    context.read<InteractionBloc>().add(
      StringInputCompleteEvent(value: _controller.text),
    );
  }

  Future<void> _onAttach(String value) async {
    var file = null as String?;
    final initialDirectory =
        BlocProvider.of<InitialDirectoryCubit>(context).state.initialDirectory;
    switch (value) {
      case 'pick_file':
        file = await FileHelper.uploadFile(initialDirectory: initialDirectory);
        break;
      case 'pick_directory':
        file = await FileHelper.uploadDirectory(
          initialDirectory: initialDirectory,
        );
        break;
      case 'save_file':
        file = await FileHelper.saveFile(initialDirectory: initialDirectory);
        break;
    }
    if (file != null) {
      _controller.text = file;
    }
  }

  Widget _buildContextMenu(
    BuildContext context,
    EditableTextState editableTextState,
  ) {
    final List<ContextMenuButtonItem> buttonItems =
        editableTextState.contextMenuButtonItems;
    buttonItems.addAll([
      ContextMenuButtonItem(
        onPressed: () async {
          await _onAttach('pick_file');
          ContextMenuController.removeAny();
        },
        label: context.los.upload_file,
      ),
      ContextMenuButtonItem(
        onPressed: () async {
          await _onAttach('pick_directory');
          ContextMenuController.removeAny();
        },
        label: context.los.upload_directory,
      ),
      ContextMenuButtonItem(
        onPressed: () async {
          await _onAttach('save_file');
          ContextMenuController.removeAny();
        },
        label: context.los.save_file,
      ),
    ]);

    return AdaptiveTextSelectionToolbar.buttonItems(
      anchors: editableTextState.contextMenuAnchors,
      buttonItems: buttonItems,
    );
  }

  List<PopupMenuEntry<String>> _buildOptionList(BuildContext context) {
    return <PopupMenuEntry<String>>[
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
    ];
  }

  Widget _buildTextField() {
    return Expanded(
      child: Card(
        child: Padding(
          padding: EdgeInsets.symmetric(
            horizontal: MediaQuery.sizeOf(context).width > 800 ? 8.0 : 4.0,
            vertical: MediaQuery.sizeOf(context).width > 800 ? 8.0 : 4.0,
          ),
          child: TextField(
            minLines: 1,
            maxLines: null,
            keyboardType: TextInputType.multiline,
            focusNode: _focusNode,
            controller: _controller,
            contextMenuBuilder: _buildContextMenu,
            decoration: InputDecoration(
              border: InputBorder.none,
              labelText: '${context.los.input_value}...',
              suffixIcon: PopupMenuButton<String>(
                initialValue: '',
                elevation: 4.0,
                icon: const Icon(Symbols.attach_file, size: 24.0),
                tooltip: context.los.attach,
                onSelected: _onAttach,
                itemBuilder: _buildOptionList,
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
        context.read<AddOptionBloc>().add(
          ExportLogEvent(messageBloc: context.read<MessageBloc>()),
        );
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
        onDragDone: (details) {
          if (details.files.isNotEmpty) {
            _controller.text = details.files.first.path;
          }
        },
        child: KeyboardListener(
          focusNode: _keyboardNode,
          onKeyEvent: (event) {
            if (event is KeyDownEvent &&
                event.logicalKey == LogicalKeyboardKey.enter) {
              _onSend();
            }
          },
          child: Row(
            spacing: 10.0,
            children: [
              _buildPopupMenuButton(),
              _buildTextField(),
              _buildActionButton(),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildPopupMenuButton() {
    return Container(
      height: 46.0,
      width: 46.0,
      margin: const EdgeInsets.only(left: 8.0),
      child: PopupMenuButton<String>(
        key: _popupMenuKey,
        tooltip: context.los.add,
        itemBuilder:
            (context) => [
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
    );
  }

  Widget _buildActionButton() {
    return Container(
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
    );
  }
}
