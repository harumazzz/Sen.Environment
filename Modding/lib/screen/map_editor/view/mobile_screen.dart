import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:pie_menu/pie_menu.dart';
import '../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../bloc/section/section_bloc.dart';
import '../bloc/setting/setting_bloc.dart';
import 'navigation.dart';
import 'option.dart';
import 'stage.dart';
import 'toolbar.dart';
import '../widgets/extension_widget.dart';
import '../widgets/screen_shortcut.dart';
import '../widgets/shop_widget.dart';

class MobileScreen extends StatelessWidget {
  const MobileScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final colorScheme = Theme.of(context).colorScheme;
    final backgroundColor =
        Theme.of(context).brightness == Brightness.dark
            ? colorScheme.onPrimaryFixedVariant
            : colorScheme.primaryFixedDim;
    final cubit = context.read<MapEditorConfigurationCubit>().state;
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
        // tooltipCanvasAlignment: Alignment.bottomCenter,
        overlayColor: Colors.transparent,
      ),
      child: BlocBuilder<SectionBloc, SectionState>(
        buildWhen: (prev, state) => prev.navigator != state.navigator,
        builder: (context, state) {
          return Column(
            children: [
              Expanded(
                child: Padding(
                  padding: const EdgeInsets.symmetric(horizontal: 12),
                  child: SizedBox(
                    width: double.infinity,
                    child: Card.outlined(
                      clipBehavior: Clip.antiAlias,
                      shape: const RoundedRectangleBorder(
                        borderRadius: BorderRadius.all(Radius.circular(20)),
                        side: BorderSide(
                          color: Color.fromRGBO(0, 0, 0, 0.3),
                          width: 2,
                        ),
                      ),
                      child: BlocBuilder<SettingBloc, SettingState>(
                        buildWhen:
                            (prev, state) =>
                                prev.showTopScreenShortcut !=
                                state.showTopScreenShortcut,
                        builder: (context, settingState) {
                          return Stack(
                            fit: StackFit.passthrough,
                            children: [
                              const MapStageView(),
                              if (state.navigator != NavigationType.option)
                                const ShopWidget(),
                              if (state.navigator != NavigationType.option)
                                const ExtensionWidget(),
                              if (settingState.showTopScreenShortcut)
                                const ScreenShortcut(),
                            ],
                          );
                        },
                      ),
                    ),
                  ),
                ),
              ),
              if (state.navigator == NavigationType.option)
                Expanded(
                  child: Card(
                    color: Theme.of(context).colorScheme.onInverseSurface,
                    margin: const EdgeInsets.symmetric(
                      vertical: 8,
                      horizontal: 16,
                    ),
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(12),
                    ),
                    elevation: 2,
                    child: NavigationView(navigator: state.navigator),
                  ),
                ),
              if (state.navigator != NavigationType.option)
                SizedBox(
                  height: 70,
                  child: Card(
                    color: Theme.of(context).colorScheme.onInverseSurface,
                    margin: const EdgeInsets.symmetric(
                      vertical: 8,
                      horizontal: 16,
                    ),
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(12),
                    ),
                    elevation: 2,
                    child: NavigationView(navigator: state.navigator),
                  ),
                ),
              NavigationBar(
                destinations: [
                  ...NavigationType.values.map((e) {
                    final item = cubit.navigationItem[e]!;
                    return NavigationDestination(
                      icon: Icon(item.icon),
                      label: item.title,
                      tooltip: item.description,
                    );
                  }),
                ],
                selectedIndex: NavigationType.values.indexOf(state.navigator),
                onDestinationSelected: (index) {
                  context.read<SectionBloc>().add(
                    SetNavigator(navigator: NavigationType.values[index]),
                  );
                },
              ),
            ],
          );
        },
      ),
    );
  }
}

class NavigationView extends StatelessWidget {
  const NavigationView({super.key, required this.navigator});

  final NavigationType navigator;

  @override
  Widget build(BuildContext context) {
    final state = context.read<MapEditorConfigurationCubit>().state;
    switch (navigator) {
      case NavigationType.tool:
        {
          return Padding(
            padding: const EdgeInsets.symmetric(horizontal: 4),
            child: ToolBarView(toolItem: state.toolItem),
          );
        }
      case NavigationType.item:
        {
          return Padding(
            padding: const EdgeInsets.symmetric(horizontal: 6, vertical: 2),
            child: ListView(
              scrollDirection: Axis.horizontal,
              children: [
                SectionView(sectionItem: state.sectionItem),
                const Padding(
                  padding: EdgeInsets.symmetric(vertical: 4),
                  child: VerticalDivider(thickness: 0.5),
                ),
                ExtensionView(extensionItem: state.extensionItem),
              ],
            ),
          );
        }
      case NavigationType.option:
        {
          return const OptionTabView();
        }
    }
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(EnumProperty<NavigationType>('navigator', navigator));
  }
}
