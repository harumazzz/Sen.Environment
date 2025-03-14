import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import '../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../bloc/resource/resource_bloc.dart';
import '../bloc/setting/setting_bloc.dart';
import '../bloc/stage/stage_bloc.dart';
import '../include/itemstore_view.dart';

class MapView extends StatelessWidget {
  const MapView({
    super.key,
    required this.cubit,
    required this.stageBloc,
    required this.resourceBloc,
    required this.settingBloc,
  });

  final MapEditorConfigurationCubit cubit;

  final StageBloc stageBloc;

  final ResourceBloc resourceBloc;

  final SettingBloc settingBloc;

  @override
  Widget build(BuildContext context) {
    final boundingRect = stageBloc.state.boundingRect;
    return ColoredBox(
      color: settingBloc.state.boundingColor,
      child: SizedBox(
        width: boundingRect.width.toDouble(),
        height: boundingRect.height.toDouble(),
        child: ItemStoreView(
          cubit: cubit,
          stageBloc: stageBloc,
          resourceBloc: resourceBloc,
          settingBloc: settingBloc,
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<MapEditorConfigurationCubit>('cubit', cubit),
    );
    properties.add(DiagnosticsProperty<StageBloc>('stageBloc', stageBloc));
    properties.add(
      DiagnosticsProperty<ResourceBloc>('resourceBloc', resourceBloc),
    );
    properties.add(
      DiagnosticsProperty<SettingBloc>('settingBloc', settingBloc),
    );
  }
}
