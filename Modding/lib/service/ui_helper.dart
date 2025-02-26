import 'package:drop_down_list/drop_down_list.dart';
import 'package:drop_down_list/model/selected_list_item.dart';
import 'package:flutter/material.dart';
import 'package:sen/model/select_option.dart';
import 'dart:typed_data';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/extension/l10n.dart';
import 'package:sen/service/file_helper.dart';

class UIHelper {
  static void showDropDownModal<T>({
    required BuildContext context,
    required List<SelectedListItem<T>> data,
    void Function(List<SelectedListItem<T>>)? onTap,
  }) {
    DropDownState<T>(
      dropDown: DropDown<T>(
        isDismissible: true,
        bottomSheetTitle: Text(
          context.los.select_an_option,
          style: const TextStyle(
            fontWeight: FontWeight.bold,
            fontSize: 20.0,
          ),
        ),
        data: data,
        onSelected: onTap,
        searchHintText: context.los.input_value,
      ),
    ).showModal(context);
  }

  static List<SelectedListItem<T>> makeSelectedItems<T>({
    required List<T> data,
    required SelectedListItem<T> Function(T data) transformation,
  }) {
    final result = <SelectedListItem<T>>[];
    for (final e in data) {
      result.add(transformation(e));
    }
    return result;
  }

  static List<SelectedListItem<T>> makeDefaultItems<T>({
    required List<T> data,
  }) {
    return makeSelectedItems<T>(
      data: data,
      transformation: (e) => SelectedListItem<T>(data: e),
    );
  }

  static List<T> toItemList<T>(
    List<SelectedListItem<T>> selectedItems,
  ) {
    final result = <T>[];
    for (final item in selectedItems) {
      result.add(item.data);
    }
    return result;
  }

  static List<SelectOption> makeEnumerationOption(
    List<String> arguments,
  ) {
    final destination = <SelectOption>[];
    for (final e in arguments) {
      final pair = e.split('|');
      destination.add(SelectOption(option: pair[0], title: pair[1]));
    }
    return destination;
  }

  static Future<void> showDetailDialog({
    required BuildContext context,
    required Widget title,
    required Widget content,
  }) async {
    await showDialog(
      context: context,
      builder: (context) => buildDialog(
        context: context,
        title: title,
        content: content,
        actions: buildSimpleAction(context: context),
      ),
    );
  }

  static List<Widget> buildSimpleAction({
    required BuildContext context,
  }) {
    return [
      TextButton(
        child: Text(context.los.okay),
        onPressed: () {
          Navigator.of(context).pop();
        },
      )
    ];
  }

  static Widget buildDialog({
    required BuildContext context,
    required Widget title,
    required Widget content,
    required List<Widget> actions,
  }) {
    return AlertDialog(
      title: title,
      content: content,
      actions: actions,
    );
  }

  static Future<void> showSimpleDialog({
    required BuildContext context,
    required String title,
    required String content,
  }) async {
    return await showDetailDialog(
      context: context,
      title: Text(title),
      content: Text(content),
    );
  }

  static Future<void> showScreenshotDialog(
    BuildContext context,
    Uint8List image,
  ) async {
    final los = context.los;
    return await showDialog(
      context: context,
      builder: (context) => buildDialog(
        context: context,
        title: Text(los.save),
        content: Image.memory(image),
        actions: [
          TextButton(
            child: Text(los.save),
            onPressed: () async {
              void closeDialog() => Navigator.of(context).pop();
              var file = await FileHelper.saveFile(
                initialDirectory: BlocProvider.of<InitialDirectoryCubit>(context).state.initialDirectory,
                suggestedName: 'screenshot.png',
              );
              if (file != null) {
                if (RegExp(r'\.png$', caseSensitive: false).hasMatch(file)) {
                  file += '.png';
                }
                FileHelper.writeBuffer(source: file, data: image);
                closeDialog();
              }
            },
          ),
          ...buildSimpleAction(context: context),
        ],
      ),
    );
  }
}
