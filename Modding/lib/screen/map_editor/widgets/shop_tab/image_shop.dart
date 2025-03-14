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

class ImageShopView extends StatelessWidget {
  const ImageShopView({super.key});

  @override
  Widget build(BuildContext context) {
    final item =
        context
            .read<MapEditorConfigurationCubit>()
            .state
            .sectionItem[SectionType.image]!;
    final los = context.los;

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
                    los.island_image_shop,
                    style: const TextStyle(fontWeight: FontWeight.bold),
                  ),
                  const Spacer(),
                  IconButton(
                    onPressed: () {
                      context.read<SectionBloc>().add(
                        const SectionMinizeToggled(
                          type: SectionType.image,
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
                child: const ImageShopGrid(),
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class ImageShopGrid extends StatelessWidget {
  const ImageShopGrid({super.key});

  @override
  Widget build(BuildContext context) {
    final los = context.los;
    return BlocBuilder<ResourceBloc, ResourceState>(
      builder: (context, state) {
        if (state.status == ResourceStateStatus.finished) {
          final shopList = state.islandImage;
          final keysList = shopList.keys.toList()..sort();
          return GridView.builder(
            itemCount: keysList.length,
            padding: const EdgeInsets.all(6),
            gridDelegate: const SliverGridDelegateWithMaxCrossAxisExtent(
              maxCrossAxisExtent: 140,
            ),
            itemBuilder: (context, index) {
              final imageId = keysList[index];
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
                            isAnimation: false,
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
                      child: RawImage(image: shopList[imageId]),
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
