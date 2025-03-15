import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../../../extension/platform.dart';
import '../../../../model/worldmap.dart';
import '../../../../extension/context.dart';
import '../../bloc/canvas/canvas_bloc.dart';
import '../../bloc/history/history_bloc.dart';
import '../../bloc/item/item_bloc.dart';
import '../../bloc/layer/layer_bloc.dart';
import '../../bloc/stage/stage_bloc.dart';
import '../../include/check_box_field.dart';
import '../../include/dropdown_button_field.dart';
import '../../include/number_text_field.dart';
import '../../include/readonly_box_field.dart';
import '../../models/action_service.dart';
import '../../models/map_const.dart';
import '../../models/action_model.dart';

class IslandConfigTab extends StatelessWidget {
  const IslandConfigTab({super.key, required this.id});

  final String id;

  @override
  Widget build(BuildContext context) {
    final piece = context.read<StageBloc>().state.pieces[id]!;
    final stageBloc = context.read<StageBloc>();
    final itemBloc = context.read<ItemBloc>();
    final historyBloc = context.read<HistoryBloc>();
    void capturePiece(ActionType actionType) {
      final actionService = ActionService<ActionModelType>(
        actionType: actionType,
        data: {ActionModelType.onePiece: piece.clone(), ActionModelType.id: id},
        change: (data) {
          final pieceId = data![ActionModelType.id];
          stageBloc.state.pieces[pieceId] =
              (data[ActionModelType.onePiece] as MapPieceItem).clone();
          itemBloc.add(const ItemStoreUpdated());
          historyBloc.add(const UpdateIndexEvent());
        },
      );
      historyBloc.add(CaptureState(state: actionService));
    }

    final los = context.los;
    final controller = context.read<CanvasBloc>().state.canvasController;
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    final spacing = isDesktopPlatform ? 0.0 : 30.0;
    const edgeSpacing = EdgeInsets.fromLTRB(12, 12, 20, 12);
    return Padding(
      padding: EdgeInsets.symmetric(horizontal: spacing),
      child: Column(
        children: [
          Card(
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(8),
            ),
            color: Theme.of(context).colorScheme.secondaryContainer,
            margin: const EdgeInsets.all(8),
            child: SizedBox(
              height: 50,
              width: double.infinity,
              child: Center(
                child: Text(
                  los.island_config,
                  style: const TextStyle(fontWeight: FontWeight.bold),
                ),
              ),
            ),
          ),
          Expanded(
            child: Padding(
              padding: const EdgeInsets.only(
                top: 4,
                left: 8,
                right: 8,
                bottom: 16,
              ),
              child: SingleChildScrollView(
                child: Column(
                  children: [
                    Row(
                      children: [
                        Expanded(
                          flex: 3,
                          child: Padding(
                            padding: edgeSpacing,
                            child: ReadonlyBoxField(
                              label: los.type,
                              value: piece.pieceType.name,
                            ),
                          ),
                        ),
                        Expanded(
                          flex: 2,
                          child: Padding(
                            padding: edgeSpacing,
                            child: NumberTextField(
                              label: los.id,
                              controller: NumberEditingController(
                                value: piece.imageID,
                                isDouble: false,
                              ),
                              range: const Range(
                                begin: 1,
                                end: MapConst.intMaxValue,
                              ),
                              useChangeButton: true,
                              onFieldSubmitted: (value) {
                                piece.imageID = value as int;
                                capturePiece(ActionType.islandChangeID);
                                itemBloc.add(const ItemStoreUpdated());
                                controller.focusNode.requestFocus();
                              },
                            ),
                          ),
                        ),
                      ],
                    ),
                    BlocBuilder<ItemBloc, ItemState>(
                      buildWhen:
                          (prev, state) => prev.itemStore != state.itemStore,
                      builder: (context, state) {
                        return Row(
                          children: [
                            Expanded(
                              child: Padding(
                                padding: edgeSpacing,
                                child: NumberTextField(
                                  label: los.position_x,
                                  controller: NumberEditingController(
                                    value: piece.position.x,
                                    isDouble: true,
                                  ),
                                  range: const Range(
                                    begin: MapConst.doubleMinValue,
                                    end: MapConst.doubleMaxValue,
                                  ),
                                  useChangeButton: true,
                                  onFieldSubmitted: (value) {
                                    piece.position.x = value as double;
                                    capturePiece(ActionType.moveItem);
                                    itemBloc.add(const ItemStoreUpdated());
                                    controller.focusNode.requestFocus();
                                  },
                                ),
                              ),
                            ),
                            Expanded(
                              child: Padding(
                                padding: edgeSpacing,
                                child: NumberTextField(
                                  label: los.position_y,
                                  controller: NumberEditingController(
                                    value: piece.position.y,
                                    isDouble: true,
                                  ),
                                  range: const Range(
                                    begin: MapConst.doubleMinValue,
                                    end: MapConst.doubleMaxValue,
                                  ),
                                  useChangeButton: true,
                                  onFieldSubmitted: (value) {
                                    piece.position.y = value as double;
                                    capturePiece(ActionType.moveItem);
                                    itemBloc.add(const ItemStoreUpdated());
                                    controller.focusNode.requestFocus();
                                  },
                                ),
                              ),
                            ),
                          ],
                        );
                      },
                    ),
                    BlocBuilder<ItemBloc, ItemState>(
                      buildWhen:
                          (prev, state) => prev.itemStore != state.itemStore,
                      builder: (context, state) {
                        return Row(
                          children: [
                            Expanded(
                              child: Padding(
                                padding: edgeSpacing,
                                child: NumberTextField(
                                  label: los.scale_x,
                                  controller: NumberEditingController(
                                    value: piece.scaleX,
                                    isDouble: true,
                                  ),
                                  range: const Range(begin: 0.1, end: 10.0),
                                  useChangeButton: true,
                                  changeStep: 0.1,
                                  onFieldSubmitted: (value) {
                                    piece.scaleX = value as double;
                                    capturePiece(ActionType.islandScale);
                                    itemBloc.add(const ItemStoreUpdated());
                                    controller.focusNode.requestFocus();
                                  },
                                ),
                              ),
                            ),
                            Expanded(
                              child: Padding(
                                padding: edgeSpacing,
                                child: NumberTextField(
                                  label: los.scale_y,
                                  controller: NumberEditingController(
                                    value: piece.scaleY,
                                    isDouble: true,
                                  ),
                                  range: const Range(begin: 0.1, end: 10.0),
                                  useChangeButton: true,
                                  changeStep: 0.1,
                                  onFieldSubmitted: (value) {
                                    piece.scaleY = value as double;
                                    capturePiece(ActionType.islandScale);
                                    itemBloc.add(const ItemStoreUpdated());
                                    controller.focusNode.requestFocus();
                                  },
                                ),
                              ),
                            ),
                          ],
                        );
                      },
                    ),
                    Row(
                      children: [
                        BlocBuilder<ItemBloc, ItemState>(
                          buildWhen:
                              (prev, state) =>
                                  prev.itemStore != state.itemStore,
                          builder: (context, state) {
                            return BlocBuilder<LayerBloc, LayerState>(
                              buildWhen:
                                  (prev, state) =>
                                      prev.treeController.roots.firstOrNull !=
                                      state.treeController.roots.firstOrNull,
                              builder: (context, state) {
                                final node = state.treeController.roots.first;
                                final layerNameList =
                                    node.children.values
                                        .map((e) => e.title)
                                        .toList();
                                return Expanded(
                                  child: Padding(
                                    padding: edgeSpacing,
                                    child: DropdownButtonField<String>(
                                      label: los.layer,
                                      value: node.children[piece.layer]!.title,
                                      items:
                                          layerNameList
                                              .map(
                                                (e) => DropdownMenuItem<String>(
                                                  value: e,
                                                  child: Text(
                                                    e,
                                                    maxLines: 1,
                                                    overflow:
                                                        TextOverflow.ellipsis,
                                                    softWrap: false,
                                                    style: Theme.of(context)
                                                        .textTheme
                                                        .bodyLarge
                                                        ?.copyWith(
                                                          color:
                                                              Theme.of(context)
                                                                  .colorScheme
                                                                  .onSurface,
                                                        ),
                                                  ),
                                                ),
                                              )
                                              .toList(),
                                      onChanged: (value) {
                                        for (final layerIndex
                                            in node.children.keys) {
                                          if (node
                                                  .children[layerIndex]!
                                                  .title ==
                                              value) {
                                            final oldLayerIndex = piece.layer;
                                            piece.layer = layerIndex;
                                            context
                                                .read<LayerBloc>()
                                                .updateNodeParallax(layerIndex);
                                            context
                                                .read<LayerBloc>()
                                                .updateNodeParallax(
                                                  oldLayerIndex,
                                                );
                                            break;
                                          }
                                        }
                                        context.read<LayerBloc>().updateTree(
                                          refresh: true,
                                        );
                                        capturePiece(
                                          ActionType.islandChangeLayer,
                                        );
                                        controller.focusNode.requestFocus();
                                      },
                                    ),
                                  ),
                                );
                              },
                            );
                          },
                        ),
                        Expanded(
                          child: Padding(
                            padding: edgeSpacing,
                            child: NumberTextField(
                              label: los.parallax,
                              controller: NumberEditingController(
                                value: piece.parallax,
                                isDouble: false,
                              ),
                              range: MapConst.parallax,
                              useChangeButton: true,
                              onFieldSubmitted: (value) {
                                piece.parallax = value as int;
                                capturePiece(ActionType.islandChangeParallax);
                                context.read<LayerBloc>().updateNodeParallax(
                                  piece.layer,
                                );
                                context.read<LayerBloc>().updateTree(
                                  refresh: true,
                                );
                                controller.focusNode.requestFocus();
                              },
                            ),
                          ),
                        ),
                      ],
                    ),
                    Row(
                      children: [
                        BlocBuilder<ItemBloc, ItemState>(
                          buildWhen:
                              (prev, state) =>
                                  prev.itemStore != state.itemStore,
                          builder: (context, state) {
                            return Expanded(
                              child: Padding(
                                padding: edgeSpacing,
                                child: NumberTextField(
                                  label: los.angle,
                                  controller: NumberEditingController(
                                    value:
                                        piece.rotationAngle * MapConst.pi / 180,
                                    isDouble: true,
                                  ),
                                  range: const Range(
                                    begin: MapConst.doubleMinValue,
                                    end: MapConst.doubleMaxValue,
                                  ),
                                  useChangeButton: true,
                                  changeStep: 0.1,
                                  onFieldSubmitted: (value) {
                                    final degrees =
                                        (value as double) / MapConst.pi * 180;
                                    piece.rotationAngle = degrees.round();
                                    capturePiece(ActionType.islandRotate);
                                    itemBloc.add(const ItemStoreUpdated());
                                    controller.focusNode.requestFocus();
                                  },
                                ),
                              ),
                            );
                          },
                        ),
                        Expanded(
                          child: Padding(
                            padding: edgeSpacing,
                            child: NumberTextField(
                              label: los.rate,
                              controller: NumberEditingController(
                                value: piece.rotationRate,
                                isDouble: true,
                              ),
                              range: const Range(
                                begin: MapConst.doubleMinValue,
                                end: MapConst.doubleMaxValue,
                              ),
                              useChangeButton: true,
                              changeStep: 0.1,
                              onFieldSubmitted: (value) {
                                piece.rotationRate = value as double;
                                capturePiece(
                                  ActionType.islandChangeRotationRate,
                                );
                                itemBloc.add(const ItemStoreUpdated());
                                controller.focusNode.requestFocus();
                              },
                            ),
                          ),
                        ),
                      ],
                    ),
                    BlocBuilder<ItemBloc, ItemState>(
                      buildWhen:
                          (prev, state) => prev.itemStore != state.itemStore,
                      builder: (context, state) {
                        return CheckBoxField(
                          label: los.piece_art_flip,
                          value: piece.isArtFlipped,
                          height: 40,
                          onChanged: ({required bool? value}) {
                            piece.isArtFlipped = value ?? false;
                            itemBloc.add(const ItemStoreUpdated());
                            capturePiece(ActionType.islandArtFlip);
                            controller.focusNode.requestFocus();
                          },
                        );
                      },
                    ),
                  ],
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('id', id));
  }
}
