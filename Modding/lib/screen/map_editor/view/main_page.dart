import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:pie_menu/pie_menu.dart';
import 'package:sen/cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'package:sen/screen/map_editor/app/l10n/l10n.dart';
import 'package:sen/screen/map_editor/bloc/canvas/canvas_bloc.dart';
import 'package:sen/screen/map_editor/bloc/history/history_bloc.dart';
import 'package:sen/screen/map_editor/bloc/init_bloc/init_bloc.dart';
import 'package:sen/screen/map_editor/bloc/item/item_bloc.dart';
import 'package:sen/screen/map_editor/bloc/layer/layer_bloc.dart';
import 'package:sen/screen/map_editor/bloc/mouse_cursor/mouse_cursor_bloc.dart';
import 'package:sen/screen/map_editor/bloc/resource/resource_bloc.dart';
import 'package:sen/screen/map_editor/bloc/section/section_bloc.dart';
import 'package:sen/screen/map_editor/bloc/selected/selected_bloc.dart';
import 'package:sen/screen/map_editor/bloc/setting/setting_bloc.dart';
import 'package:sen/screen/map_editor/bloc/stage/stage_bloc.dart';
import 'package:sen/screen/map_editor/bloc/suggestion/suggestion_bloc.dart';
import 'package:sen/screen/map_editor/bloc/ticker/ticker_bloc.dart';
import 'package:sen/screen/map_editor/bloc/toolbar/toolbar_bloc.dart';
import 'package:sen/screen/map_editor/include/ticker.dart';
import 'package:sen/screen/map_editor/view/navigation.dart';
import 'package:sen/screen/map_editor/view/option.dart';
import 'package:sen/screen/map_editor/view/stage.dart';
import 'package:sen/screen/map_editor/view/toolbar.dart';
import 'package:sen/screen/map_editor/widgets/extension_widget.dart';
import 'package:sen/screen/map_editor/widgets/shop_widget.dart';
import 'package:sen/screen/map_editor/widgets/tool_dialog/clear_tool.dart';
import 'package:sen/screen/map_editor/widgets/tool_dialog/config_tool.dart';
import 'package:sen/screen/map_editor/widgets/tool_dialog/shortcut_help.dart';

class MainPage extends StatelessWidget {
  const MainPage({super.key, required this.mapEditorConfigurationCubit});

  final MapEditorConfigurationCubit mapEditorConfigurationCubit;

  @override
  Widget build(BuildContext context) {
    final los = context.los;
    return MultiBlocProvider(providers: [
      BlocProvider<MouseCursorBloc>(create: (_) => MouseCursorBloc()),
      BlocProvider<SuggestionBloc>(create: (_) => SuggestionBloc()),
      BlocProvider<SettingBloc>(create: (_) => SettingBloc()),
      BlocProvider<InitBloc>(create: (_) => InitBloc()),
      BlocProvider<SectionBloc>(create: (_) => SectionBloc()),
      BlocProvider<SelectedBloc>(create: (_) => SelectedBloc()),
      BlocProvider<HistoryBloc>(create: (_) => HistoryBloc()),
      BlocProvider<TickerBloc>(
          create: (_) => TickerBloc(
              ticker: const Ticker(),
              tickStart: 0,
              tickEnd: 360 * 30,
              tickDuration: 30)),
      BlocProvider<CanvasBloc>(create: (_) {
        final bloc = CanvasBloc();
        bloc.add(const InitCameraViewOffset());
        return bloc;
      }),
      BlocProvider<ToolBarBloc>(
        create: (context) => ToolBarBloc(
            cubit: context.read<MapEditorConfigurationCubit>(),
            settingBloc: context.read<SettingBloc>(),
            initBloc: context.read<InitBloc>(),
            los: los),
      ),
      BlocProvider<ResourceBloc>(
          create: (context) => ResourceBloc(
                cubit: context.read<MapEditorConfigurationCubit>(),
                settingBloc: context.read<SettingBloc>(),
                initBloc: context.read<InitBloc>(),
                los: los,
              )),
      BlocProvider<StageBloc>(
          create: (context) => StageBloc(
                cubit: context.read<MapEditorConfigurationCubit>(),
                selectedBloc: context.read<SelectedBloc>(),
                historyBloc: context.read<HistoryBloc>(),
                canvasBloc: context.read<CanvasBloc>(),
                resourceBloc: context.read<ResourceBloc>(),
                suggestionBloc: context.read<SuggestionBloc>(),
                sectionBloc: context.read<SectionBloc>(),
                settingBloc: context.read<SettingBloc>(),
                initBloc: context.read<InitBloc>(),
                los: los,
              )),
      BlocProvider<ItemBloc>(
          create: (context) => ItemBloc(
                cubit: context.read<MapEditorConfigurationCubit>(),
                stageBloc: context.read<StageBloc>(),
                selectedBloc: context.read<SelectedBloc>(),
                historyBloc: context.read<HistoryBloc>(),
                canvasBloc: context.read<CanvasBloc>(),
                resourceBloc: context.read<ResourceBloc>(),
                settingBloc: context.read<SettingBloc>(),
              )),
      BlocProvider<LayerBloc>(
          create: (context) => LayerBloc(
                historyBloc: context.read<HistoryBloc>(),
                selectedBloc: context.read<SelectedBloc>(),
                stageBloc: context.read<StageBloc>(),
                itemBloc: context.read<ItemBloc>(),
                los: los,
              )),
    ], child: const MainPageChild());
  }
}

class MainPageChild extends StatelessWidget {
  const MainPageChild({super.key});

  void _initBloc(BuildContext context) {
    context.read<CanvasBloc>().add(TransformControllerAddListener(
          listener: () =>
              context.read<ItemBloc>().add(const ItemStoreUpdated()),
        ));
    context.read<SuggestionBloc>().add(InitailizeSuggestionList(
          configModel:
              context.read<MapEditorConfigurationCubit>().state.configModel,
        ));
  }

  @override
  Widget build(BuildContext context) {
    if (context.read<InitBloc>().state.status == InitailizeStatus.initailize) {
      context
          .read<InitBloc>()
          .add(const SetStatusEvent(status: InitailizeStatus.success));
      _initBloc(context);
    }
    final colorScheme = Theme.of(context).colorScheme;
    final backgroundColor = Theme.of(context).brightness == Brightness.dark
        ? colorScheme.onPrimaryFixedVariant
        : colorScheme.primaryFixedDim;
    final state = context.read<MapEditorConfigurationCubit>().state;
    return BlocListener<InitBloc, InitState>(
      listenWhen: (prev, state) =>
          prev.text != state.text ||
          prev.alertDialogEnable != state.alertDialogEnable,
      listener: (context, state) async {
        if (state.text != null && state.text != 'null') {
          ScaffoldMessenger.of(context).showSnackBar(
            SnackBar(
              content: Text(state.text!),
              width: 400,
              behavior: SnackBarBehavior.floating,
              duration: const Duration(seconds: 2),
            ),
          );
        }
        if (state.alertDialogEnable[AlertDialogShowType.clear]!) {
          final done = await showDialog<bool>(
              context: context,
              builder: (context) {
                return const ClearToolWidget();
              });
          if (context.mounted) {
            context.read<InitBloc>().add(const ShowAlertDialog(
                type: AlertDialogShowType.clear, enable: false));
            if (done == true) {
              final stageBloc = context.read<StageBloc>();
              context.read<ToolBarBloc>().add(ToolClearSubmitted(
                    stageBloc: stageBloc,
                    itemUpdate: () =>
                        context.read<ItemBloc>().add(const ItemStoreUpdated()),
                    layerBloc: context.read<LayerBloc>(),
                  ));
            }
          }
        } else if (state.alertDialogEnable[AlertDialogShowType.config]!) {
          final settingBloc = context.read<SettingBloc>();
          final done = await showDialog<bool>(
              context: context,
              builder: (context) {
                return ConfigSettingWidget(
                  bloc: settingBloc,
                );
              });
          if (context.mounted) {
            context.read<InitBloc>().add(const ShowAlertDialog(
                type: AlertDialogShowType.config, enable: false));
            if (done == true) {
              context.read<ToolBarBloc>().add(const ToolConfigSubmitted());
            }
          }
        } else if (state.alertDialogEnable[AlertDialogShowType.shortcut]!) {
          await showDialog<bool>(
              context: context,
              builder: (context) {
                return const ShortCutMenuWidget();
              });
          if (context.mounted) {
            context.read<InitBloc>().add(const ShowAlertDialog(
                type: AlertDialogShowType.shortcut, enable: false));
          }
        }
      },
      child: PieCanvas(
          theme: PieTheme(
              buttonTheme: PieButtonTheme(
                  backgroundColor: colorScheme.secondaryContainer,
                  iconColor: colorScheme.inverseSurface),
              buttonThemeHovered: PieButtonTheme(
                  backgroundColor: backgroundColor,
                  iconColor: colorScheme.inverseSurface),
              delayDuration: Duration.zero,
              spacing: 4,
              radius: 60,
              angleOffset: 45,
              buttonSize: 45,
              pointerSize: 0,
              tooltipTextStyle: const TextStyle(
                inherit: false,
              ),
              // tooltipCanvasAlignment: Alignment.bottomCenter,
              overlayColor: Colors.transparent),
          child: Row(children: [
            SizedBox(
                width: 60,
                child: Padding(
                  padding: const EdgeInsets.only(top: 6, left: 6),
                  child: ToolBarView(toolItem: state.toolItem),
                )),
            Expanded(
                child: Column(
              mainAxisSize: MainAxisSize.max,
              children: [
                const Expanded(
                  child: SizedBox(
                      width: double.infinity,
                      child: Card.outlined(
                        clipBehavior: Clip.antiAlias,
                        shape: RoundedRectangleBorder(
                            borderRadius: BorderRadius.all(Radius.circular(16)),
                            side: BorderSide(
                                color: Color.fromRGBO(0, 0, 0, 0.3), width: 2)),
                        child: Stack(
                          fit: StackFit.passthrough,
                          children: [
                            MapStageView(),
                            ShopWidget(),
                            ExtensionWidget(),
                          ],
                        ),
                      )),
                ),
                Row(
                  children: [
                    Expanded(
                        child: SizedBox(
                      height: 60,
                      child: SectionView(sectionItem: state.sectionItem),
                    )),
                    SizedBox(
                      width: 240,
                      height: 60,
                      child: ExtensionView(extensionItem: state.extensionItem),
                    ),
                  ],
                )
              ],
            )),
            const SizedBox(
              width: 280,
              child: Card(
                  margin:
                      EdgeInsets.only(top: 4, left: 4, right: 8, bottom: 12),
                  shadowColor: Colors.transparent,
                  child: Padding(
                    padding: EdgeInsets.all(8),
                    child: OptionTabView(),
                  )),
            ),
          ])),
    );
  }
}
