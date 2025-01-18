
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/screen/map_editor/bloc/section/section_bloc.dart';
import 'package:sen/screen/map_editor/widgets/extension_tab/history.dart';
import 'package:sen/screen/map_editor/widgets/extension_tab/layer.dart';
import 'package:sen/screen/map_editor/widgets/extension_tab/palette.dart';
import 'package:sen/screen/map_editor/widgets/extension_tab/setting.dart';

class ExtensionWidget extends StatelessWidget {
  const ExtensionWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<SectionBloc, SectionState>(
        buildWhen: (prev, state) => prev.extension != state.extension,
        builder: (context, state) {
          final extension = state.extension;
          return Positioned(
              right: 20,
              bottom: 20,
              child: Row(
                mainAxisAlignment: MainAxisAlignment.end,
                children: [
                  Column(
                    crossAxisAlignment: CrossAxisAlignment.end,
                    children: [
                      if (extension[ExtensionType.history]!) const HistoryWidget(),
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
              ));
        });
  }
}
