import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../../extension/platform.dart';
import '../bloc/section/section_bloc.dart';
import 'extension_tab/history.dart';
import 'extension_tab/layer.dart';
import 'extension_tab/palette.dart';
import 'extension_tab/setting.dart';

class ExtensionWidget extends StatelessWidget {
  const ExtensionWidget({super.key});

  @override
  Widget build(BuildContext context) {
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    return BlocBuilder<SectionBloc, SectionState>(
      buildWhen: (prev, state) => prev.extension != state.extension,
      builder: (context, state) {
        final extension = state.extension;
        if (isDesktopPlatform) {
          final isLowScreenHeight = MediaQuery.sizeOf(context).height < 965;
          if (isLowScreenHeight &&
              extension[ExtensionType.history]! &&
              extension[ExtensionType.layer]!) {
            extension[ExtensionType.history] = false;
          }
          if (isLowScreenHeight &&
              extension[ExtensionType.palette]! &&
              extension[ExtensionType.setting]!) {
            extension[ExtensionType.palette] = false;
          }
          return Positioned(
            right: 20,
            bottom: 20,
            child: Row(
              mainAxisAlignment: MainAxisAlignment.end,
              children: [
                Column(
                  crossAxisAlignment: CrossAxisAlignment.end,
                  children: [
                    if (extension[ExtensionType.history]!)
                      const HistoryWidget(),
                    if (extension[ExtensionType.layer]!) const LayerWidget(),
                  ],
                ),
                Column(
                  crossAxisAlignment: CrossAxisAlignment.end,
                  children: [
                    if (extension[ExtensionType.palette]!)
                      const PaletteWidget(),
                    if (extension[ExtensionType.setting]!)
                      const EditorSettingWidget(),
                  ],
                ),
              ],
            ),
          );
        } else {
          return Center(
            child: Padding(
              padding: const EdgeInsets.only(bottom: 20),
              child: Column(
                mainAxisAlignment: MainAxisAlignment.end,
                children: [
                  if (extension[ExtensionType.history]!) const HistoryWidget(),
                  if (extension[ExtensionType.layer]!) const LayerWidget(),
                  if (extension[ExtensionType.palette]!) const PaletteWidget(),
                  if (extension[ExtensionType.setting]!)
                    const EditorSettingWidget(),
                ],
              ),
            ),
          );
        }
      },
    );
  }
}
