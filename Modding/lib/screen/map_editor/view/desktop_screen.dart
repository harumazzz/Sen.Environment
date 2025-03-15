import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:pie_menu/pie_menu.dart';
import '../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'navigation.dart';
import 'option.dart';
import 'stage.dart';
import 'toolbar.dart';
import '../widgets/extension_widget.dart';
import '../widgets/shop_widget.dart';

class DesktopScreen extends StatelessWidget {
  const DesktopScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final colorScheme = Theme.of(context).colorScheme;
    final backgroundColor =
        Theme.of(context).brightness == Brightness.dark
            ? colorScheme.onPrimaryFixedVariant
            : colorScheme.primaryFixedDim;
    final state = context.read<MapEditorConfigurationCubit>().state;
    return PieCanvas(
      theme: PieTheme(
        buttonTheme: PieButtonTheme(
          backgroundColor: colorScheme.secondaryContainer,
          iconColor: colorScheme.inverseSurface,
        ),
        buttonThemeHovered: PieButtonTheme(
          backgroundColor: backgroundColor,
          iconColor: colorScheme.inverseSurface,
        ),
        delayDuration: Duration.zero,
        spacing: 4,
        radius: 60,
        angleOffset: 45,
        buttonSize: 45,
        pointerSize: 0,
        tooltipTextStyle: const TextStyle(inherit: false),
        overlayColor: Colors.transparent,
      ),
      child: Row(
        children: [
          SizedBox(
            width: 60,
            child: Padding(
              padding: const EdgeInsets.only(top: 6, left: 6),
              child: ToolBarView(toolItem: state.toolItem),
            ),
          ),
          Expanded(
            child: Column(
              children: [
                const Expanded(
                  child: SizedBox(
                    width: double.infinity,
                    child: Card.outlined(
                      clipBehavior: Clip.antiAlias,
                      shape: RoundedRectangleBorder(
                        borderRadius: BorderRadius.all(Radius.circular(16)),
                        side: BorderSide(
                          color: Color.fromRGBO(0, 0, 0, 0.3),
                          width: 2,
                        ),
                      ),
                      child: Stack(
                        fit: StackFit.passthrough,
                        children: [
                          MapStageView(),
                          ShopWidget(),
                          ExtensionWidget(),
                        ],
                      ),
                    ),
                  ),
                ),
                Row(
                  children: [
                    Expanded(
                      child: SizedBox(
                        height: 60,
                        child: SectionView(sectionItem: state.sectionItem),
                      ),
                    ),
                    SizedBox(
                      width: 240,
                      height: 60,
                      child: ExtensionView(extensionItem: state.extensionItem),
                    ),
                  ],
                ),
              ],
            ),
          ),
          const SizedBox(
            width: 280,
            child: Card(
              margin: EdgeInsets.only(top: 4, left: 4, right: 8, bottom: 12),
              shadowColor: Colors.transparent,
              child: Padding(
                padding: EdgeInsets.all(8),
                child: OptionTabView(),
              ),
            ),
          ),
        ],
      ),
    );
  }
}
