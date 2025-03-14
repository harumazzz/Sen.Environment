import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../../../../extension/context.dart';
import '../../bloc/item/item_bloc.dart';
import '../../bloc/resource/resource_bloc.dart';
import '../../bloc/section/section_bloc.dart';
import '../../bloc/setting/setting_bloc.dart';
import '../../bloc/stage/stage_bloc.dart';
import '../../models/event_node.dart';

class EventShopView extends StatelessWidget {
  const EventShopView({super.key});

  @override
  Widget build(BuildContext context) {
    final item =
        context
            .read<MapEditorConfigurationCubit>()
            .state
            .sectionItem[SectionType.event]!;
    return Card(
      color: Theme.of(context).colorScheme.surface,
      child: Padding(
        padding: const EdgeInsets.only(top: 4, right: 16, left: 16, bottom: 16),
        child: Column(
          children: [
            SizedBox(
              height: 40,
              child: Row(
                children: [
                  Container(
                    margin: const EdgeInsets.symmetric(horizontal: 10),
                    child: Icon(item.icon),
                  ),
                  Text(
                    context.los.event_shop,
                    style: const TextStyle(fontWeight: FontWeight.bold),
                  ),
                  const Spacer(),
                  IconButton(
                    onPressed: () {
                      context.read<SectionBloc>().add(
                        const SectionMinizeToggled(
                          type: SectionType.event,
                          minize: true,
                        ),
                      );
                    },
                    icon: const Icon(Symbols.close),
                  ),
                ],
              ),
            ),
            Expanded(
              child: Container(
                decoration: BoxDecoration(
                  borderRadius: BorderRadius.circular(12),
                  color: Theme.of(context).colorScheme.secondaryContainer,
                ),
                child: const EventShopGrid(),
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class EventShopGrid extends StatelessWidget {
  const EventShopGrid({super.key});

  bool _checkOldEvent(EventNodeType type) {
    switch (type) {
      case EventNodeType.stargate:
      case EventNodeType.keygate:
      case EventNodeType.pathNode:
      case EventNodeType.doodad:
        return true;
      default:
        return false;
    }
  }

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<ResourceBloc, ResourceState>(
      builder: (context, state) {
        if (state.status == ResourceStateStatus.finished) {
          final shopList = state.eventShop;
          final eventName = state.eventNodeName;
          final keysList = shopList.keys.toList();
          return BlocBuilder<SettingBloc, SettingState>(
            buildWhen: (prev, state) => prev.hideOldEvent != state.hideOldEvent,
            builder: (context, state) {
              return GridView.builder(
                itemCount: keysList.length,
                padding: const EdgeInsets.all(6),
                gridDelegate: const SliverGridDelegateWithMaxCrossAxisExtent(
                  maxCrossAxisExtent: 140,
                ),
                itemBuilder: (context, index) {
                  final eventType = keysList[index];
                  if (state.hideOldEvent && _checkOldEvent(eventType)) {
                    return Card(
                      color: Colors.transparent,
                      shadowColor: Colors.transparent,
                      child: ColorFiltered(
                        colorFilter: const ColorFilter.mode(
                          Colors.grey,
                          BlendMode.modulate,
                        ),
                        child: Card(
                          margin: const EdgeInsets.all(10),
                          shadowColor: Colors.transparent,
                          clipBehavior: Clip.antiAlias,
                          child: shopList[eventType],
                        ),
                      ),
                    );
                  } else {
                    return Card(
                      color: Colors.transparent,
                      shadowColor: Colors.transparent,
                      child: Tooltip(
                        message: eventName[eventType],
                        child: InkWell(
                          borderRadius: BorderRadius.circular(16),
                          onTap:
                              () => context.read<StageBloc>().add(
                                AddEventItemEvent(
                                  eventType: eventType,
                                  itemBloc: context.read<ItemBloc>(),
                                  stageBloc: context.read<StageBloc>(),
                                ),
                              ),
                          splashColor:
                              Theme.of(context).colorScheme.secondaryFixedDim,
                          child: Card(
                            margin: const EdgeInsets.all(10),
                            shadowColor: Colors.transparent,
                            clipBehavior: Clip.antiAlias,
                            child: shopList[eventType],
                          ),
                        ),
                      ),
                    );
                  }
                },
              );
            },
          );
        } else {
          return const Center(child: CircularProgressIndicator.adaptive());
        }
      },
    );
  }
}
