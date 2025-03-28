import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../bloc/argument_bloc/argument_bloc.dart';
import '../../bloc/initial_directory_bloc/initial_directory_bloc.dart';
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

  Future<void> _onAttach(String value) async {
    var result = null as String?;
    final bloc = BlocProvider.of<InitialDirectoryBloc>(context);
    final initialDirectory = () {
      final state = bloc.state;
      if (state is InitialDirectoryLoaded) {
        return state.initialDirectory;
      } else {
        return null;
      }
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

  void _onAddArgument() {
    if (_formKey.currentState?.validate() ?? false) {
      context.read<ArgumentBloc>().add(AddArgument(value: [_controller.text]));
      _controller.clear();
      _focusNode.unfocus();
    }
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
              Row(
                spacing: 4.0,
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  CustomPrefixButton(onAttach: _onAttach),
                  CustomTextField(
                    focusNode: _focusNode,
                    textEditingController: _controller,
                    menuBuilder: _buildContextMenu,
                    onAttach: _onAttach,
                  ),
                  AddButton(onAdd: _onAddArgument),
                ],
              ),
              const SizedBox(height: 8.0),
              ...state.value.mapIndexed((index, element) {
                return CustomListTile(index: index, value: element);
              }),
            ],
          ),
        );
      },
    );
  }
}

class ClearButton extends StatelessWidget {
  const ClearButton({super.key, required this.onClear});

  final void Function() onClear;

  @override
  Widget build(BuildContext context) {
    return IconButton(onPressed: onClear, icon: const Icon(Symbols.clear));
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(ObjectFlagProperty<void Function()>.has('onClear', onClear));
  }
}

class AddButton extends StatelessWidget {
  const AddButton({super.key, required this.onAdd});

  final void Function() onAdd;

  @override
  Widget build(BuildContext context) {
    return IconButton(
      tooltip: context.los.add,
      onPressed: onAdd,
      icon: const Icon(Symbols.add),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(ObjectFlagProperty<void Function()>.has('onAdd', onAdd));
  }
}

class ButtonTile extends StatelessWidget {
  const ButtonTile({
    super.key,
    required this.child,
    required this.onPressed,
    this.useDivider = true,
  });
  final Widget child;
  final void Function() onPressed;
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
    properties.add(DiagnosticsProperty<bool>('useDivider', useDivider));
    properties.add(
      ObjectFlagProperty<void Function()>.has('onPressed', onPressed),
    );
  }
}

class CustomPrefixButton extends StatelessWidget {
  const CustomPrefixButton({super.key, required this.onAttach});

  final Future<void> Function(String value) onAttach;

  Future<void> _onDisplayOption(BuildContext context) async {
    final options = <({String name, void Function() onTap, bool useDivider})>[
      (
        name: context.los.upload_file,
        onTap: () async => await onAttach('pick_file'),
        useDivider: true,
      ),
      (
        name: context.los.upload_directory,
        onTap: () async => await onAttach('pick_directory'),
        useDivider: true,
      ),
      (
        name: context.los.save_file,
        onTap: () async => await onAttach('save_file'),
        useDivider: true,
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
                return ButtonTile(
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

  @override
  Widget build(BuildContext context) {
    return IconButton(
      tooltip: context.los.attach,
      onPressed: () async => await _onDisplayOption(context),
      icon: const Icon(Symbols.attachment),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<Future<void> Function(String value)>.has(
        'onAttach',
        onAttach,
      ),
    );
  }
}

class CustomTextField extends StatelessWidget {
  const CustomTextField({
    super.key,
    required this.focusNode,
    required this.textEditingController,
    required this.menuBuilder,
    required this.onAttach,
  });

  final FocusNode focusNode;

  final TextEditingController textEditingController;

  final Widget Function(BuildContext context, EditableTextState textState)
  menuBuilder;

  final Future<void> Function(String value) onAttach;

  @override
  Widget build(BuildContext context) {
    return Expanded(
      child: TextFormField(
        focusNode: focusNode,
        controller: textEditingController,
        contextMenuBuilder: menuBuilder,
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
          suffix: ClearButton(
            onClear: () {
              textEditingController.clear();
              focusNode.unfocus();
            },
          ),
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<
        Widget Function(BuildContext context, EditableTextState textState)
      >.has('menuBuilder', menuBuilder),
    );
    properties.add(
      DiagnosticsProperty<TextEditingController>(
        'textEditingController',
        textEditingController,
      ),
    );
    properties.add(DiagnosticsProperty<FocusNode>('focusNode', focusNode));
    properties.add(
      ObjectFlagProperty<Future<void> Function(String value)>.has(
        'onAttach',
        onAttach,
      ),
    );
  }
}

class CustomListTile extends StatelessWidget {
  const CustomListTile({super.key, required this.index, required this.value});

  final int index;

  final String value;

  void _onRemove(BuildContext context) {
    context.read<ArgumentBloc>().add(RemoveArgument(index: index));
  }

  @override
  Widget build(BuildContext context) {
    return Dismissible(
      key: ValueKey(index),
      background: Container(
        decoration: BoxDecoration(
          borderRadius: BorderRadius.circular(16.0),
          color: Colors.red,
        ),
      ),
      onDismissed: (_) => _onRemove(context),
      child: ListTile(
        contentPadding: const EdgeInsets.symmetric(
          horizontal: 12.0,
          vertical: 2.0,
        ),
        tileColor: Theme.of(context).colorScheme.surfaceContainerHighest,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(16.0),
        ),
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
        title: SelectableText(
          value,
          style: Theme.of(context).textTheme.labelLarge?.copyWith(
            color: Theme.of(context).colorScheme.onPrimaryContainer,
          ),
        ),
        trailing: IconButton(
          icon: const Icon(Symbols.close),
          tooltip: context.los.delete,
          onPressed: () => _onRemove(context),
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(IntProperty('index', index));
    properties.add(StringProperty('value', value));
  }
}
