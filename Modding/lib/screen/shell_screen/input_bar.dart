import 'package:desktop_drop/desktop_drop.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../bloc/add_option_bloc/add_option_bloc.dart';
import '../../bloc/interaction_bloc/interaction_bloc.dart';
import '../../bloc/message_bloc/message_bloc.dart';
import '../../bloc/initial_directory_bloc/initial_directory_bloc.dart';
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

  @override
  void initState() {
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

  Future<void> _onDisplayOption() async {
    final options = [
      (
        name: context.los.upload_file,
        onTap: () async => await _onAttach('pick_file'),
        useDivider: true,
      ),
      (
        name: context.los.upload_directory,
        onTap: () async => await _onAttach('pick_directory'),
        useDivider: true,
      ),
      (
        name: context.los.save_file,
        onTap: () async => await _onAttach('save_file'),
        useDivider: true,
      ),
      (
        name: context.los.take_screenshot,
        onTap: () => _onAdd('screen_shot'),
        useDivider: true,
      ),
      (
        name: context.los.export_log,
        onTap: () => _onAdd('export_log'),
        useDivider: false,
      ),
    ];

    await showModalBottomSheet(
      context: context,
      shape: const RoundedRectangleBorder(
        borderRadius: BorderRadius.vertical(top: Radius.circular(16.0)),
      ),
      builder: (context) {
        return Padding(
          padding: const EdgeInsets.symmetric(vertical: 8.0),
          child: ListView(
            shrinkWrap: true,
            children: [
              ...options.map((e) {
                return ValueButton(
                  onPressed: () {
                    e.onTap();
                    Navigator.of(context).pop();
                  },
                  useDivider: e.useDivider,
                  child: Text(e.name),
                );
              }),
            ],
          ),
        );
      },
    );
  }

  void _onSend() {
    _focusNode.unfocus();
    context.read<InteractionBloc>().add(
      StringInputCompleteEvent(value: _controller.text),
    );
  }

  Future<void> _onAttach(String value) async {
    var result = null as String?;
    final bloc = BlocProvider.of<InitialDirectoryBloc>(context);
    final initialDirectory = () {
      if (bloc.state is InitialDirectoryLoaded) {
        return (bloc.state as InitialDirectoryLoaded).initialDirectory;
      }
      return null;
    }();
    switch (value) {
      case 'pick_file':
        result = await FileHelper.uploadFile(
          initialDirectory: initialDirectory,
        );
        break;
      case 'pick_directory':
        result = await FileHelper.uploadDirectory(
          initialDirectory: initialDirectory,
        );
        break;
      case 'save_file':
        result = await FileHelper.saveFile(initialDirectory: initialDirectory);
        break;
    }
    if (result != null) {
      _controller.text = result;
      if (value == 'pick_directory') {
        bloc.add(SetDirectoryOfDirectory(source: result));
      } else {
        bloc.add(SetDirectoryOfFile(source: result));
      }
    }
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
              CustomTextField(
                focusNode: _focusNode,
                controller: _controller,
                onSend: _onSend,
                onDisplay: _onDisplayOption,
                onAttach: _onAttach,
              ),
            ],
          ),
        ),
      ),
    );
  }
}

class AddButton extends StatelessWidget {
  const AddButton({super.key, required this.onDisplay});

  final void Function() onDisplay;

  @override
  Widget build(BuildContext context) {
    return Container(
      height: 46.0,
      width: 46.0,
      margin: const EdgeInsets.only(left: 8.0),
      child: IconButton(
        tooltip: context.los.add,
        onPressed: onDisplay,
        icon: const Icon(Symbols.add),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<void Function()>.has('onDisplay', onDisplay),
    );
  }
}

class ActionButton extends StatelessWidget {
  const ActionButton({super.key, required this.onSend});

  final void Function() onSend;

  @override
  Widget build(BuildContext context) {
    return Container(
      height: 46.0,
      width: 46.0,
      margin: const EdgeInsets.only(right: 8.0),
      decoration: BoxDecoration(
        color: Theme.of(context).colorScheme.primaryContainer,
        shape: BoxShape.circle,
      ),
      child: FloatingActionButton(
        heroTag: 'send-btn',
        tooltip: context.los.submit,
        elevation: 4.0,
        onPressed: onSend,
        child: const Icon(Symbols.send),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(ObjectFlagProperty<void Function()>.has('onSend', onSend));
  }
}

class ValueButton extends StatelessWidget {
  const ValueButton({
    super.key,
    required this.onPressed,
    required this.child,
    required this.useDivider,
  });

  final void Function() onPressed;

  final Widget child;

  final bool useDivider;

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        ListTile(onTap: onPressed, title: child),
        if (useDivider) const Divider(),
      ],
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<void Function()>.has('onPressed', onPressed),
    );
    properties.add(DiagnosticsProperty<bool>('useDivider', useDivider));
  }
}

class CustomTextField extends StatelessWidget {
  const CustomTextField({
    super.key,
    required this.focusNode,
    required this.controller,
    required this.onSend,
    required this.onDisplay,
    required this.onAttach,
  });

  final FocusNode focusNode;

  final TextEditingController controller;

  final void Function() onSend;
  final void Function() onDisplay;

  final Future<void> Function(String value) onAttach;

  Widget _buildContextMenu(
    BuildContext context,
    EditableTextState editableTextState,
  ) {
    final List<ContextMenuButtonItem> buttonItems =
        editableTextState.contextMenuButtonItems;
    buttonItems.addAll([
      ContextMenuButtonItem(
        onPressed: () async {
          await onAttach('pick_file');
          ContextMenuController.removeAny();
        },
        label: context.los.upload_file,
      ),
      ContextMenuButtonItem(
        onPressed: () async {
          await onAttach('pick_directory');
          ContextMenuController.removeAny();
        },
        label: context.los.upload_directory,
      ),
      ContextMenuButtonItem(
        onPressed: () async {
          await onAttach('save_file');
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

  @override
  Widget build(BuildContext context) {
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
            focusNode: focusNode,
            controller: controller,
            contextMenuBuilder: _buildContextMenu,
            decoration: InputDecoration(
              border: InputBorder.none,
              labelText: '${context.los.input_value}...',
              suffixIcon: ActionButton(onSend: onSend),
              prefixIcon: AddButton(onDisplay: onDisplay),
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

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(ObjectFlagProperty<void Function()>.has('onSend', onSend));
    properties.add(DiagnosticsProperty<FocusNode>('focusNode', focusNode));
    properties.add(
      DiagnosticsProperty<TextEditingController>('controller', controller),
    );
    properties.add(
      ObjectFlagProperty<void Function()>.has('onDisplay', onDisplay),
    );
    properties.add(
      ObjectFlagProperty<Future<void> Function(String value)>.has(
        'onAttach',
        onAttach,
      ),
    );
  }
}
