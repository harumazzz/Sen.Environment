import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:screenshot/screenshot.dart';
import '../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../../../extension/context.dart';
import '../../../extension/platform.dart';
import '../../../service/ui_helper.dart';
import '../bloc/autosave/autosave_bloc.dart';
import '../bloc/canvas/canvas_bloc.dart';
import '../bloc/history/history_bloc.dart';
import '../bloc/init_bloc/init_bloc.dart';
import '../bloc/item/item_bloc.dart';
import '../bloc/layer/layer_bloc.dart';
import '../bloc/mouse_cursor/mouse_cursor_bloc.dart';
import '../bloc/resource/resource_bloc.dart';
import '../bloc/section/section_bloc.dart';
import '../bloc/selected/selected_bloc.dart';
import '../bloc/setting/setting_bloc.dart';
import '../bloc/shortcut/shortcut_bloc.dart';
import '../bloc/stage/stage_bloc.dart';
import '../bloc/suggestion/suggestion_bloc.dart';
import '../bloc/ticker/ticker_bloc.dart';
import '../bloc/toolbar/toolbar_bloc.dart';
import '../include/ticker.dart';
import 'map.dart';
import 'desktop_screen.dart';
import 'mobile_screen.dart';
import '../widgets/tool_dialog/clear_tool.dart';
import '../widgets/tool_dialog/config_tool.dart';
import '../widgets/tool_dialog/shortcut_help.dart';

class MainPage extends StatelessWidget {
  const MainPage({super.key, required this.mapEditorConfigurationCubit});

  final MapEditorConfigurationCubit mapEditorConfigurationCubit;

  @override
  Widget build(BuildContext context) {
    final los = context.los;
    return MultiBlocProvider(
      providers: [
        BlocProvider<MouseCursorBloc>(create: (_) => MouseCursorBloc()),
        BlocProvider<SuggestionBloc>(create: (_) => SuggestionBloc()),
        BlocProvider<SettingBloc>(create: (_) => SettingBloc()),
        BlocProvider<SectionBloc>(create: (_) => SectionBloc()),
        BlocProvider<SelectedBloc>(create: (_) => SelectedBloc()),
        BlocProvider<HistoryBloc>(create: (_) => HistoryBloc()),
        BlocProvider<TickerBloc>(
          create:
              (_) => TickerBloc(
                ticker: const Ticker(),
                tickStart: 0,
                tickEnd: 360 * 30,
                tickDuration: 30,
              ),
        ),
        BlocProvider<CanvasBloc>(
          create: (_) {
            final bloc = CanvasBloc();
            bloc.add(const InitCameraViewOffset());
            return bloc;
          },
        ),
        BlocProvider<ToolBarBloc>(
          create:
              (context) => ToolBarBloc(
                cubit: context.read<MapEditorConfigurationCubit>(),
                settingBloc: context.read<SettingBloc>(),
                initBloc: context.read<InitBloc>(),
                los: los,
              ),
        ),
        BlocProvider<ResourceBloc>(
          create:
              (context) => ResourceBloc(
                cubit: context.read<MapEditorConfigurationCubit>(),
                settingBloc: context.read<SettingBloc>(),
                initBloc: context.read<InitBloc>(),
                los: los,
              ),
        ),
        BlocProvider<StageBloc>(
          create:
              (context) => StageBloc(
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
              ),
        ),
        BlocProvider<AutosaveBloc>(
          create: (_) => AutosaveBloc(stageBloc: context.read<StageBloc>()),
        ),
        BlocProvider<ItemBloc>(
          create:
              (context) => ItemBloc(
                cubit: context.read<MapEditorConfigurationCubit>(),
                stageBloc: context.read<StageBloc>(),
                selectedBloc: context.read<SelectedBloc>(),
                historyBloc: context.read<HistoryBloc>(),
                canvasBloc: context.read<CanvasBloc>(),
                resourceBloc: context.read<ResourceBloc>(),
                settingBloc: context.read<SettingBloc>(),
              ),
        ),
        BlocProvider<LayerBloc>(
          create:
              (context) => LayerBloc(
                historyBloc: context.read<HistoryBloc>(),
                selectedBloc: context.read<SelectedBloc>(),
                stageBloc: context.read<StageBloc>(),
                itemBloc: context.read<ItemBloc>(),
                los: los,
              ),
        ),
        BlocProvider<ShortcutBloc>(
          create:
              (context) => ShortcutBloc(
                stageBloc: context.read<StageBloc>(),
                layerBloc: context.read<LayerBloc>(),
                selectedBloc: context.read<SelectedBloc>(),
                itemBloc: context.read<ItemBloc>(),
                historyBloc: context.read<HistoryBloc>(),
                settingBloc: context.read<SettingBloc>(),
                cubit: context.read<MapEditorConfigurationCubit>(),
              ),
        ),
      ],
      child: const MainPageChild(),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<MapEditorConfigurationCubit>(
        'mapEditorConfigurationCubit',
        mapEditorConfigurationCubit,
      ),
    );
  }
}

class MainPageChild extends StatelessWidget {
  const MainPageChild({super.key});

  void _initBloc(BuildContext context) {
    context.read<CanvasBloc>().add(
      TransformControllerAddListener(
        listener: () => context.read<ItemBloc>().add(const ItemStoreUpdated()),
      ),
    );
    context.read<SuggestionBloc>().add(
      InitailizeSuggestionList(
        configModel:
            context.read<MapEditorConfigurationCubit>().state.configModel,
      ),
    );
    context.read<InitBloc>().add(
      CreateTakeShootFunction(
        takeShoot: () async {
          return ScreenshotController().captureFromLongWidget(
            MapView(
              stageBloc: context.read<StageBloc>(),
              cubit: context.read<MapEditorConfigurationCubit>(),
              settingBloc: context.read<SettingBloc>(),
              resourceBloc: context.read<ResourceBloc>(),
            ),
            delay: const Duration(milliseconds: 100),
          );
        },
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    if (context.read<InitBloc>().state.status == InitailizeStatus.initailize) {
      context.read<InitBloc>().add(
        const SetStatusEvent(status: InitailizeStatus.success),
      );
      _initBloc(context);
    }
    return BlocListener<InitBloc, InitState>(
      listenWhen:
          (prev, state) =>
              prev.text != state.text ||
              prev.alertDialogEnable != state.alertDialogEnable,
      listener: (context, state) async {
        if (state.text != null && state.text != 'null') {
          ScaffoldMessenger.of(context).showSnackBar(
            SnackBar(
              content: Text(
                state.text!,
                softWrap: true,
                overflow: TextOverflow.visible,
              ),
              behavior: SnackBarBehavior.floating,
              duration: const Duration(milliseconds: 1500),
              width: UIHelper.ofDesktop(builder: () => 400.0),
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(10.0),
              ),
            ),
          );
        }
        if (state.alertDialogEnable[AlertDialogShowType.clear] != null &&
            state.alertDialogEnable[AlertDialogShowType.clear]!) {
          final done = await showDialog<bool>(
            context: context,
            builder: (context) {
              return const ClearToolWidget();
            },
          );
          if (context.mounted) {
            context.read<InitBloc>().add(
              const ShowAlertDialog(
                type: AlertDialogShowType.clear,
                enable: false,
              ),
            );
            if (done == true) {
              final stageBloc = context.read<StageBloc>();
              context.read<ToolBarBloc>().add(
                ToolClearSubmitted(
                  stageBloc: stageBloc,
                  itemUpdate:
                      () => context.read<ItemBloc>().add(
                        const ItemStoreUpdated(),
                      ),
                  layerBloc: context.read<LayerBloc>(),
                ),
              );
            }
          }
        } else if (state.alertDialogEnable[AlertDialogShowType.config]!) {
          final settingBloc = context.read<SettingBloc>();
          final done = await showDialog<bool>(
            context: context,
            builder: (context) {
              return ConfigSettingWidget(bloc: settingBloc);
            },
          );
          if (context.mounted) {
            context.read<InitBloc>().add(
              const ShowAlertDialog(
                type: AlertDialogShowType.config,
                enable: false,
              ),
            );
            if (done == true) {
              context.read<ToolBarBloc>().add(const ToolConfigSubmitted());
            }
          }
        } else if (state.alertDialogEnable[AlertDialogShowType.shortcut]!) {
          await showDialog<bool>(
            context: context,
            builder: (context) {
              return const ShortCutMenuWidget();
            },
          );
          if (context.mounted) {
            context.read<InitBloc>().add(
              const ShowAlertDialog(
                type: AlertDialogShowType.shortcut,
                enable: false,
              ),
            );
          }
        }
      },
      child:
          (CurrentPlatform.isMobile)
              ? const MobileScreen()
              : const DesktopScreen(),
    );
  }
}
