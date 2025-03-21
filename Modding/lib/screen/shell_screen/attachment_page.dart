import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';

import '../../bloc/add_option_bloc/add_option_bloc.dart';
import '../../bloc/argument_bloc/argument_bloc.dart';
import '../../bloc/message_bloc/message_bloc.dart';
import '../../cubit/initial_directory_cubit/initial_directory_cubit.dart';
import '../../extension/context.dart';
import 'package:collection/collection.dart';

import '../../service/file_helper.dart';

class AttachmentPage extends StatefulWidget {
  const AttachmentPage({super.key});

  @override
  State<AttachmentPage> createState() => _AttachmentPageState();
}

class _AttachmentPageState extends State<AttachmentPage> {
  late TextEditingController _controller;

  late FocusNode _focusNode;

  late GlobalKey<FormState> _formKey;

  @override
  void initState() {
    _controller = TextEditingController();
    _focusNode = FocusNode();
    _formKey = GlobalKey<FormState>();
    super.initState();
  }

  @override
  void dispose() {
    _controller.dispose();
    _focusNode.dispose();
    super.dispose();
  }

  Widget _buildAdd() {
    return IconButton(
      tooltip: context.los.add,
      onPressed: () {
        if (!_formKey.currentState!.validate()) {
          return;
        }
        context.read<ArgumentBloc>().add(
          AddArgument(value: [_controller.text]),
        );
        _controller.clear();
        _focusNode.unfocus();
      },
      icon: const Icon(Symbols.add),
    );
  }

  Widget _buildClear() {
    return IconButton(
      onPressed: () {
        _controller.clear();
        _focusNode.unfocus();
      },
      icon: const Icon(Symbols.clear),
    );
  }

  Widget _buildListTile(int index, String value) {
    return ListTile(
      contentPadding: const EdgeInsets.symmetric(
        horizontal: 12.0,
        vertical: 2.0,
      ),
      tileColor: Theme.of(context).colorScheme.surfaceContainerHighest,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16.0)),
      leading: CircleAvatar(
        backgroundColor: Theme.of(context).colorScheme.primaryContainer,
        child: Text(
          '${index + 1}',
          style: Theme.of(context).textTheme.labelLarge?.copyWith(
            fontWeight: FontWeight.bold,
            color: Theme.of(context).colorScheme.onPrimaryContainer,
          ),
        ),
      ),
      title: Text(
        value,
        style: Theme.of(context).textTheme.labelLarge?.copyWith(
          color: Theme.of(context).colorScheme.onPrimaryContainer,
        ),
      ),
      trailing: IconButton(
        icon: const Icon(Symbols.close),
        tooltip: context.los.delete,
        onPressed: () {
          context.read<ArgumentBloc>().add(RemoveArgument(index: index));
        },
      ),
    );
  }

  Widget _buildPrefix() {
    return IconButton(
      onPressed: _onDisplayOption,
      icon: const Icon(Symbols.attachment),
    );
  }

  Future<void> _onAttach(String value) async {
    var result = null as String?;
    final cubit = BlocProvider.of<InitialDirectoryCubit>(context);
    final initialDirectory = cubit.state.initialDirectory;
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
        cubit.setDirectoryOfDirectory(source: result);
      } else {
        cubit.setDirectoryOfFile(source: result);
      }
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

  Widget _buttonOf({
    required Widget child,
    required void Function() onPressed,
    bool useDivider = true,
  }) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        ListTile(onTap: onPressed, title: child),
        if (useDivider) const Divider(),
      ],
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

  Future<void> _onDisplayOption() async {
    final options = <({String name, void Function() onTap, bool useDivider})>[
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
              ...options.map(
                (e) => _buttonOf(
                  onPressed: () {
                    pop() => Navigator.of(context).pop();
                    e.onTap();
                    pop();
                  },
                  child: Text(e.name),
                  useDivider: e.useDivider,
                ),
              ),
            ],
          ),
        );
      },
    );
  }

  Widget _buildTextField() {
    return Expanded(
      child: TextFormField(
        focusNode: _focusNode,
        controller: _controller,
        contextMenuBuilder: _buildContextMenu,
        validator: (value) {
          if (value == null || value.trim().isEmpty) {
            return context.los.value_cannot_be_empty;
          }
          return null;
        },
        minLines: 1,
        maxLines: null,
        decoration: InputDecoration(
          labelText: context.los.input_value,
          contentPadding: const EdgeInsets.only(
            left: 8.0,
            top: 8.0,
            bottom: 8.0,
          ),
          border: const OutlineInputBorder(),
          suffix: _buildClear(),
          prefix: _buildPrefix(),
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<ArgumentBloc, ArgumentState>(
      buildWhen: (previous, current) => previous != current,
      builder: (context, state) {
        return Form(
          key: _formKey,
          child: Column(
            spacing: 4.0,
            children: [
              Row(spacing: 4.0, children: [_buildTextField(), _buildAdd()]),
              const SizedBox(height: 8.0),
              ...state.value.mapIndexed(_buildListTile),
            ],
          ),
        );
      },
    );
  }
}
