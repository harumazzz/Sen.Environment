import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../../../../extension/context.dart';
import '../../bloc/item/item_bloc.dart';
import '../../bloc/layer/layer_bloc.dart';
import '../../bloc/resource/resource_bloc.dart';
import '../../bloc/section/section_bloc.dart';
import '../../bloc/stage/stage_bloc.dart';
import '../animation_widget.dart';

class AnimationShopView extends StatelessWidget {
  const AnimationShopView({super.key});

  @override
  Widget build(BuildContext context) {
    final los = context.los;
    final item =
        context
            .read<MapEditorConfigurationCubit>()
            .state
            .sectionItem[SectionType.animation]!;
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
                    los.animation_pieces_shop,
                    style: const TextStyle(fontWeight: FontWeight.bold),
                  ),
                  const Spacer(),
                  IconButton(
                    onPressed: () {
                      context.read<SectionBloc>().add(
                        const SectionMinizeToggled(
                          type: SectionType.animation,
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
                child: const AnimationShopGrid(),
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class AnimationShopGrid extends StatelessWidget {
  const AnimationShopGrid({super.key});

  @override
  Widget build(BuildContext context) {
    final los = context.los;
    return BlocBuilder<ResourceBloc, ResourceState>(
      builder: (context, state) {
        if (state.status == ResourceStateStatus.finished) {
          final shopList = context.read<ResourceBloc>().state.islandAnimation;
          final keysList = shopList.keys.toList()..sort();
          return GridView.builder(
            itemCount: keysList.length,
            padding: const EdgeInsets.all(6),
            gridDelegate: const SliverGridDelegateWithMaxCrossAxisExtent(
              maxCrossAxisExtent: 140,
            ),
            itemBuilder: (context, index) {
              final imageId = keysList[index];
              final visual = shopList[imageId]!;
              return Card(
                color: Colors.transparent,
                shadowColor: Colors.transparent,
                child: Tooltip(
                  message: '${los.island} $imageId',
                  child: InkWell(
                    borderRadius: BorderRadius.circular(16),
                    onTap:
                        () => context.read<StageBloc>().add(
                          AddIslandItemEvent(
                            imageId: imageId,
                            isAnimation: true,
                            layerBloc: context.read<LayerBloc>(),
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
                      child: FittedBox(
                        child: UnconstrainedBox(
                          child: SizedOverflowBox(
                            alignment: AlignmentDirectional.topStart,
                            size: visual.visualSize,
                            child: AnimationWidget(
                              visual: visual,
                              labelPlay: const ['main'],
                              borderWidth: 0,
                            ),
                          ),
                        ),
                      ),
                    ),
                  ),
                ),
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
