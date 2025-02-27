import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:screenshot/screenshot.dart';
import 'package:sen/cubit/initial_directory_cubit/initial_directory_cubit.dart';
import 'package:sen/cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import 'package:sen/extension/context.dart';
import 'package:sen/screen/map_editor/bloc/autosave/autosave_bloc.dart';
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
import 'package:sen/screen/map_editor/bloc/shortcut/shortcut_bloc.dart';
import 'package:sen/screen/map_editor/bloc/stage/stage_bloc.dart';
import 'package:sen/screen/map_editor/bloc/suggestion/suggestion_bloc.dart';
import 'package:sen/screen/map_editor/bloc/ticker/ticker_bloc.dart';
import 'package:sen/screen/map_editor/bloc/toolbar/toolbar_bloc.dart';
import 'package:sen/screen/map_editor/include/ticker.dart';
import 'package:sen/screen/map_editor/view/map.dart';
import 'package:sen/screen/map_editor/view/desktop_screen.dart';
import 'package:sen/screen/map_editor/view/mobile_screen.dart';
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
      BlocProvider<MouseCursorBloc>(
        create: (_) => MouseCursorBloc(),
      ),
      BlocProvider<SuggestionBloc>(
        create: (_) => SuggestionBloc(),
      ),
      BlocProvider<SettingBloc>(
        create: (_) => SettingBloc(),
      ),
      BlocProvider<SectionBloc>(
        create: (_) => SectionBloc(),
      ),
      BlocProvider<SelectedBloc>(
        create: (_) => SelectedBloc(),
      ),
      BlocProvider<HistoryBloc>(
        create: (_) => HistoryBloc(),
      ),
      BlocProvider<TickerBloc>(
        create: (_) => TickerBloc(ticker: const Ticker(), tickStart: 0, tickEnd: 360 * 30, tickDuration: 30),
      ),
      BlocProvider<CanvasBloc>(
        create: (_) {
          final bloc = CanvasBloc();
          bloc.add(const InitCameraViewOffset());
          return bloc;
        },
      ),
      BlocProvider<ToolBarBloc>(
        create: (context) => ToolBarBloc(
          cubit: context.read<MapEditorConfigurationCubit>(),
          settingBloc: context.read<SettingBloc>(),
          initBloc: context.read<InitBloc>(),
          los: los,
          initialDirectoryCubit: context.read<InitialDirectoryCubit>(),
        ),
      ),
      BlocProvider<ResourceBloc>(
        create: (context) => ResourceBloc(
          cubit: context.read<MapEditorConfigurationCubit>(),
          settingBloc: context.read<SettingBloc>(),
          initBloc: context.read<InitBloc>(),
          los: los,
        ),
      ),
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
        ),
      ),
      BlocProvider<AutosaveBloc>(
        create: (_) => AutosaveBloc(
          stageBloc: context.read<StageBloc>(),
        ),
      ),
      BlocProvider<ItemBloc>(
        create: (context) => ItemBloc(
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
        create: (context) => LayerBloc(
          historyBloc: context.read<HistoryBloc>(),
          selectedBloc: context.read<SelectedBloc>(),
          stageBloc: context.read<StageBloc>(),
          itemBloc: context.read<ItemBloc>(),
          los: los,
        ),
      ),
      BlocProvider<ShortcutBloc>(
          create: (context) => ShortcutBloc(
              stageBloc: context.read<StageBloc>(),
              layerBloc: context.read<LayerBloc>(),
              selectedBloc: context.read<SelectedBloc>(),
              itemBloc: context.read<ItemBloc>(),
              historyBloc: context.read<HistoryBloc>(),
              settingBloc: context.read<SettingBloc>(),
              cubit: context.read<MapEditorConfigurationCubit>()))
    ], child: const MainPageChild());
  }
}

class MainPageChild extends StatelessWidget {
  const MainPageChild({super.key});

  void _initBloc(BuildContext context) {
    context.read<CanvasBloc>().add(TransformControllerAddListener(
          listener: () => context.read<ItemBloc>().add(const ItemStoreUpdated()),
        ));
    context.read<SuggestionBloc>().add(InitailizeSuggestionList(
          configModel: context.read<MapEditorConfigurationCubit>().state.configModel,
        ));
    context.read<InitBloc>().add(CreateTakeShootFunction(takeShoot: () async {
      return ScreenshotController().captureFromLongWidget(
          MapView(
            stageBloc: context.read<StageBloc>(),
            cubit: context.read<MapEditorConfigurationCubit>(),
            settingBloc: context.read<SettingBloc>(),
            resourceBloc: context.read<ResourceBloc>(),
          ),
          delay: const Duration(milliseconds: 100));
    }));
  }

  @override
  Widget build(BuildContext context) {
    if (context.read<InitBloc>().state.status == InitailizeStatus.initailize) {
      context.read<InitBloc>().add(const SetStatusEvent(status: InitailizeStatus.success));
      _initBloc(context);
    }
    return BlocListener<InitBloc, InitState>(
        listenWhen: (prev, state) => prev.text != state.text || prev.alertDialogEnable != state.alertDialogEnable,
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
              context.read<InitBloc>().add(const ShowAlertDialog(type: AlertDialogShowType.clear, enable: false));
              if (done == true) {
                final stageBloc = context.read<StageBloc>();
                context.read<ToolBarBloc>().add(ToolClearSubmitted(
                      stageBloc: stageBloc,
                      itemUpdate: () => context.read<ItemBloc>().add(const ItemStoreUpdated()),
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
              context.read<InitBloc>().add(const ShowAlertDialog(type: AlertDialogShowType.config, enable: false));
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
              context.read<InitBloc>().add(const ShowAlertDialog(type: AlertDialogShowType.shortcut, enable: false));
            }
          }
        },
        child: (Platform.isAndroid || Platform.isIOS) ? const MobileScreen() : const DesktopScreen());
  }
}
