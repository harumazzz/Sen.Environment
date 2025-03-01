import 'package:drop_down_list/drop_down_list.dart';
import 'package:drop_down_list/model/selected_list_item.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/model/select_option.dart';
import 'dart:typed_data';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/extension/context.dart';
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
        title: title,
        content: content,
        actions: buildSimpleAction(context: context),
      ),
    );
  }

  static Future<void> showCustomDialog({
    required BuildContext context,
    required Widget child,
  }) async {
    await showDialog(
      context: context,
      builder: (context) => Dialog(
        backgroundColor: Colors.transparent,
        insetPadding: const EdgeInsets.all(16.0),
        child: SizedBox(
          width: MediaQuery.of(context).size.width * 0.85,
          height: MediaQuery.of(context).size.height * 0.6,
          child: child,
        ),
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

  static Future<void> showFlutterDialog({
    required BuildContext context,
    required Widget child,
  }) async {
    return await showDialog(
      context: context,
      builder: (context) => child,
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

  static Future<void> showWidget({
    required BuildContext context,
    required String name,
    required Widget child,
  }) async {
    await showDialog(
      context: context,
      barrierColor: Colors.black.withValues(alpha: 0.6),
      builder: (context) => Dialog(
        backgroundColor: Theme.of(context).colorScheme.surfaceContainerHighest,
        child: Stack(
          children: [
            Positioned.fill(
              child: InteractiveViewer(
                panEnabled: true,
                minScale: 1.0,
                maxScale: 5.0,
                child: Center(
                  child: ClipRRect(
                    borderRadius: BorderRadius.circular(16.0),
                    child: child,
                  ),
                ),
              ),
            ),
            Positioned(
              top: 12.0,
              right: 12.0,
              child: SafeArea(
                child: IconButton(
                  onPressed: () => Navigator.pop(context),
                  icon: Icon(
                    Symbols.close,
                    color: Theme.of(context).colorScheme.secondary,
                  ),
                ),
              ),
            ),
            Positioned(
              top: 16.0,
              left: 16.0,
              child: SafeArea(
                child: Container(
                  padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 6),
                  child: Text(
                    name,
                    textAlign: TextAlign.center,
                    style: Theme.of(context).textTheme.titleMedium?.copyWith(fontWeight: FontWeight.w600),
                  ),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  static Future<void> showFullImage({
    required BuildContext context,
    required String name,
    required ImageProvider image,
  }) async {
    return await showWidget(
      context: context,
      name: name,
      child: Image(image: image),
    );
  }

  static Color colorOf(BuildContext context) {
    return context.isDynamicColor
        ? Theme.of(context).colorScheme.surfaceContainer
        : (context.isDarkMode ? Colors.grey[850]! : Colors.white);
  }
}
