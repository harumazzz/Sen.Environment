// ignore_for_file: avoid_positional_boolean_parameters

import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../extension/platform.dart';
import '../model/select_option.dart';
import 'dart:typed_data';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../cubit/initial_directory_cubit/initial_directory_cubit.dart';
import '../extension/context.dart';
import '../widget/material_dialog.dart';
import 'file_helper.dart';

class UIHelper {
  const UIHelper._();

  static List<SelectOption> makeEnumerationOption(List<String> arguments) {
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
      builder:
          (context) => buildDialog(
            title: title,
            content: content,
            actions: buildSimpleAction(context: context),
          ),
    );
  }

  static Future<void> showCustomDialog({
    required BuildContext context,
    required Widget child,
    bool barrierDismissible = true,
  }) async {
    await showDialog(
      context: context,
      barrierDismissible: barrierDismissible,
      builder:
          (context) => Dialog(
            backgroundColor: Colors.transparent,
            insetPadding: const EdgeInsets.all(16.0),
            child: SizedBox(
              width: MediaQuery.sizeOf(context).width * 0.85,
              height: MediaQuery.sizeOf(context).height * 0.6,
              child: child,
            ),
          ),
    );
  }

  static List<Widget> buildSimpleAction({required BuildContext context}) {
    return [
      TextButton(
        child: Text(context.los.okay),
        onPressed: () {
          Navigator.of(context).pop();
        },
      ),
    ];
  }

  static Widget buildDialog({
    required Widget title,
    required Widget content,
    required List<Widget> actions,
  }) {
    return MaterialDialog(title: title, content: content, actions: actions);
  }

  static Future<void> showSimpleDialog({
    required BuildContext context,
    required String title,
    required String content,
  }) async {
    return await showDetailDialog(
      context: context,
      title: Text(
        title,
        style: Theme.of(
          context,
        ).textTheme.titleMedium?.copyWith(fontWeight: FontWeight.bold),
      ),
      content: Text(content, overflow: TextOverflow.ellipsis, maxLines: 4),
    );
  }

  static Future<void> showFlutterDialog({
    required BuildContext context,
    required Widget child,
  }) async {
    return await showDialog(context: context, builder: (context) => child);
  }

  static Future<void> showScreenshotDialog(
    BuildContext context,
    Uint8List image,
  ) async {
    final los = context.los;
    return await showDialog(
      context: context,
      builder:
          (context) => buildDialog(
            title: Text(los.save),
            content: InteractiveViewer(
              minScale: 1.0,
              maxScale: 5.0,
              child: Center(
                child: ClipRRect(
                  borderRadius: BorderRadius.circular(16.0),
                  child: Image.memory(image),
                ),
              ),
            ),
            actions: [
              TextButton(
                child: Text(los.save),
                onPressed: () async {
                  void closeDialog() => Navigator.of(context).pop();
                  var file = await FileHelper.saveFile(
                    initialDirectory:
                        BlocProvider.of<InitialDirectoryCubit>(
                          context,
                        ).state.initialDirectory,
                    suggestedName: 'screenshot.png',
                  );
                  if (file != null) {
                    if (RegExp(
                      r'\.png$',
                      caseSensitive: false,
                    ).hasMatch(file)) {
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
      builder:
          (context) => Dialog(
            backgroundColor:
                Theme.of(context).colorScheme.surfaceContainerHighest,
            child: Stack(
              children: [
                Positioned.fill(
                  child: InteractiveViewer(
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
                      padding: const EdgeInsets.symmetric(
                        horizontal: 12,
                        vertical: 6,
                      ),
                      child: Text(
                        name,
                        textAlign: TextAlign.center,
                        style: Theme.of(context).textTheme.titleMedium
                            ?.copyWith(fontWeight: FontWeight.w600),
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

  static T? ofDesktop<T>({required T Function() builder}) {
    if (CurrentPlatform.isDesktop) {
      return builder();
    }
    return null;
  }

  static T? ofMobile<T>({required T Function() builder}) {
    if (CurrentPlatform.isMobile) {
      return builder();
    }
    return null;
  }

  static Widget buildTrailingReturn(BuildContext context) {
    return IconButton(
      tooltip: context.los.back,
      onPressed: () {
        Navigator.of(context).pop();
      },
      icon: const Icon(Symbols.arrow_back),
    );
  }

  static List<Widget> buildAppBar(
    BuildContext context,
    bool innerBoxIsScrolled, {
    Widget? leading,
    Widget? title,
    List<Widget>? actions,
  }) {
    final hasLeading =
        leading != null || Navigator.of(context).canPop() ? true : false;
    return <Widget>[
      SliverAppBar(
        pinned: true,
        expandedHeight: kToolbarHeight + 50,
        backgroundColor: Theme.of(context).cardColor,
        centerTitle: false,
        leading: leading,
        actions: actions,
        flexibleSpace: LayoutBuilder(
          builder: (BuildContext context, BoxConstraints constraints) {
            final isCollapsed = constraints.maxHeight <= kToolbarHeight + 32;
            return Container(
              decoration: BoxDecoration(
                color:
                    !isCollapsed
                        ? Colors.transparent
                        : () {
                          if (context.isDarkMode) {
                            return Colors.grey.shade900;
                          }
                          return Theme.of(context).colorScheme.primaryContainer
                              .withValues(alpha: 0.48);
                        }(),
              ),
              child: FlexibleSpaceBar(
                titlePadding: EdgeInsets.only(
                  left: isCollapsed && hasLeading ? 48.0 : 20.0,
                  bottom: 12.0,
                ),
                expandedTitleScale: 1.3,
                title: title,
              ),
            );
          },
        ),
      ),
    ];
  }

  static Widget applyScrollView({
    required List<Widget> Function(BuildContext, bool) builder,
    required Widget child,
  }) {
    if (CurrentPlatform.isDesktop) {
      return child;
    }
    return NestedScrollView(headerSliverBuilder: builder, body: child);
  }
}
