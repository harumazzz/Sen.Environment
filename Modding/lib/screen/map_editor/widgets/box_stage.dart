import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:matrix4_transform/matrix4_transform.dart';
import 'package:pie_menu/pie_menu.dart';
import '../../../extension/platform.dart';
import '../../../model/worldmap.dart';
import '../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../bloc/canvas/canvas_bloc.dart';
import '../bloc/item/item_bloc.dart';
import '../bloc/stage/stage_bloc.dart';
import '../bloc/ticker/ticker_bloc.dart';
import '../include/painter.dart';
import '../include/rectangle_box.dart';
import '../models/config.dart';
import '../models/game_resource.dart';
import '../models/map_const.dart';

class BoxStage extends StatelessWidget {
  const BoxStage({
    super.key,
    required this.mapGrid,
    required this.useEraseTool,
    required this.usePanTool,
    required this.useResizeTool,
    this.boundBackground = BorderBackground.color,
    this.boxStageColor = Colors.black,
    required this.pieMenuAction,
    required this.children,
  });

  final bool useEraseTool;

  final bool usePanTool;

  final bool useResizeTool;

  final bool mapGrid;

  final BorderBackground boundBackground;

  final Color boxStageColor;

  final List<PieAction> pieMenuAction;

  final List<Widget> children;

  Widget _resizeBox(BuildContext context) {
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    final safeAdditionalWidth =
        isDesktopPlatform
            ? MapConst.safeDesktopAdditionalWidth
            : MapConst.safeMobileAdditionalWidth;
    final safeAdditionalHeight =
        isDesktopPlatform
            ? MapConst.safeDesktopAdditionalHeight
            : MapConst.safeMobileAdditionalHeight;
    final startPositionX = safeAdditionalWidth / 2;
    final startPositionY = safeAdditionalHeight / 2;
    final boundingRect = context.read<StageBloc>().state.boundingRect;
    return RectangleBox(
      minWidth: MapConst.minBoundingWidth.toDouble(),
      minHeight: MapConst.minBoundingHeight.toDouble(),
      boundingRect: Rect.fromLTWH(
        startPositionX - 4,
        startPositionY - 4,
        boundingRect.width.toDouble() + 8,
        boundingRect.height.toDouble() + 8,
      ),
      onScalingEnd: (updateRect) {
        final newX = boundingRect.x + (updateRect.left - startPositionX);
        final newY = boundingRect.y + (updateRect.top - startPositionY);
        final bound = BoundingRect(
          x: newX.round(),
          y: newY.round(),
          width: updateRect.width.round() - 8,
          height: updateRect.height.round() - 8,
        );
        context.read<StageBloc>().add(UpdateBoundingRect(boundingRect: bound));
        context.read<ItemBloc>().add(const ItemStoreUpdated());
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    final colorScheme = Theme.of(context).colorScheme;
    final backgroundColor =
        Theme.of(context).brightness == Brightness.dark
            ? colorScheme.onPrimaryFixedVariant
            : colorScheme.primaryFixedDim;
    final transformationController =
        context
            .read<CanvasBloc>()
            .state
            .canvasController
            .transformationController;
    final double kDrag = isDesktopPlatform ? 0.0000135 : 0.005;
    var stage =
        mapGrid
            ? GridPaper(
              interval: 200,
              divisions: 1,
              child: InteractiveViewer.builder(
                panEnabled: usePanTool,
                scaleEnabled: usePanTool,
                minScale: 0.5,
                maxScale: 3,
                scaleFactor: 800,
                transformationController: transformationController,
                interactionEndFrictionCoefficient: kDrag,
                builder: (context, quad) {
                  final child = OverlayWithRectangleClipping(
                    child: Stack(fit: StackFit.passthrough, children: children),
                  );
                  if (useResizeTool) {
                    return Stack(children: [child, _resizeBox(context)]);
                  } else {
                    return child;
                  }
                },
              ),
            )
            : InteractiveViewer.builder(
              panEnabled: usePanTool,
              scaleEnabled: usePanTool,
              minScale: 0.5,
              maxScale: 3,
              scaleFactor: 800,
              transformationController: transformationController,
              interactionEndFrictionCoefficient: kDrag,
              builder: (context, quad) {
                final child = OverlayWithRectangleClipping(
                  child: Stack(fit: StackFit.passthrough, children: children),
                );
                if (useResizeTool) {
                  return Stack(children: [child, _resizeBox(context)]);
                } else {
                  return child;
                }
              },
            );
    if (!useEraseTool && !usePanTool) {
      stage = PieMenu(
        theme: PieTheme(
          buttonTheme: PieButtonTheme(
            backgroundColor: colorScheme.secondaryContainer,
            iconColor: colorScheme.inverseSurface,
          ),
          buttonThemeHovered: PieButtonTheme(
            backgroundColor: backgroundColor,
            iconColor: colorScheme.inverseSurface,
          ),
          delayDuration:
              isDesktopPlatform
                  ? Duration.zero
                  : const Duration(milliseconds: 300),
          spacing: 4,
          radius: 60,
          angleOffset: 45,
          buttonSize: 45,
          pointerSize: 0,
          tooltipTextStyle: const TextStyle(inherit: false),
          childBounceEnabled: false,
          overlayStyle: PieOverlayStyle.around,
          childOpacityOnButtonHover: 1,
          overlayColor: Colors.transparent,
          leftClickShowsMenu: !isDesktopPlatform,
          rightClickShowsMenu: isDesktopPlatform,
        ),
        actions: pieMenuAction,
        child: stage,
      );
    }

    switch (boundBackground) {
      case BorderBackground.timeSpace:
        {
          final mainState = context.read<MapEditorConfigurationCubit>().state;
          final spaceSpiral =
              mainState.gameResource.commonImage[ImageCommonType.spaceSpiral]!;
          final spaceDust =
              mainState.gameResource.commonImage[ImageCommonType.spaceDust]!;
          return ColoredBox(
            color: boxStageColor,
            child: Stack(
              fit: StackFit.passthrough,
              children: [
                RotaionWidget(
                  rotationRate: 0.05,
                  scale: 1.9,
                  child: RawImage(image: spaceSpiral, fit: BoxFit.cover),
                ),
                RawImage(image: spaceDust, fit: BoxFit.cover),
                Column(
                  children: [
                    Expanded(
                      child: Container(
                        decoration: const BoxDecoration(
                          gradient: LinearGradient(
                            begin: Alignment.topCenter,
                            end: Alignment.bottomCenter,
                            colors: [
                              Colors.black,
                              Colors.black54,
                              Colors.transparent,
                            ],
                            stops: [0.4, 0.7, 0.9],
                            tileMode: TileMode.decal,
                          ),
                        ),
                      ),
                    ),
                    const Expanded(flex: 2, child: SizedBox()),
                    Expanded(
                      child: Container(
                        decoration: const BoxDecoration(
                          gradient: LinearGradient(
                            begin: Alignment.bottomCenter,
                            end: Alignment.topCenter,
                            colors: [
                              Colors.black,
                              Colors.black54,
                              Colors.transparent,
                            ],
                            stops: [0.4, 0.7, 0.8],
                            tileMode: TileMode.decal,
                          ),
                        ),
                      ),
                    ),
                  ],
                ),
                stage,
              ],
            ),
          );
        }
      default:
        {
          return ColoredBox(color: boxStageColor, child: stage);
        }
    }
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<bool>('useEraseTool', useEraseTool));
    properties.add(DiagnosticsProperty<bool>('usePanTool', usePanTool));
    properties.add(DiagnosticsProperty<bool>('useResizeTool', useResizeTool));
    properties.add(DiagnosticsProperty<bool>('mapGrid', mapGrid));
    properties.add(
      EnumProperty<BorderBackground>('boundBackground', boundBackground),
    );
    properties.add(ColorProperty('boxStageColor', boxStageColor));
    properties.add(IterableProperty<PieAction>('pieMenuAction', pieMenuAction));
  }
}

class OverlayWithRectangleClipping extends StatelessWidget {
  const OverlayWithRectangleClipping({super.key, required this.child});

  final Widget child;

  @override
  Widget build(BuildContext context) {
    final boundingRect = BlocProvider.of<StageBloc>(context).state.boundingRect;

    final isDesktopPlatform = CurrentPlatform.isDesktop;
    final safeAdditionalWidth =
        isDesktopPlatform
            ? MapConst.safeDesktopAdditionalWidth
            : MapConst.safeMobileAdditionalWidth;
    final safeAdditionalHeight =
        isDesktopPlatform
            ? MapConst.safeDesktopAdditionalHeight
            : MapConst.safeMobileAdditionalHeight;
    final maxBoundWidth = safeAdditionalWidth + boundingRect.width;
    final maxBoundHeight = safeAdditionalHeight + boundingRect.height;
    final innerRect = Rect.fromLTWH(
      safeAdditionalWidth / 2,
      safeAdditionalHeight / 2,
      boundingRect.width.toDouble(),
      boundingRect.height.toDouble(),
    );
    final outlineRect = Rect.fromLTWH(
      0,
      0,
      maxBoundWidth * 2,
      maxBoundHeight * 2,
    );
    return SizedBox(
      width: maxBoundWidth.toDouble(),
      height: maxBoundHeight.toDouble(),
      child: CustomPaint(
        foregroundPainter: RectanglePainter(
          innerRect: innerRect,
          outlineRect: outlineRect,
        ),
        child: Stack(fit: StackFit.passthrough, children: [child]),
      ),
    );
  }
}

class RotaionWidget extends StatelessWidget {
  const RotaionWidget({
    super.key,
    required this.scale,
    required this.rotationRate,
    required this.child,
  });

  final Widget child;

  final double rotationRate;

  final double scale;

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<TickerBloc, TickerState>(
      builder: (context, state) {
        final rotateDeg = (state.tick * rotationRate * 1.7) % 360;
        return Transform(
          alignment: Alignment.center,
          transform:
              Matrix4Transform().scale(scale).rotateDegrees(-rotateDeg).m,
          child: child,
        );
      },
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DoubleProperty('rotationRate', rotationRate));
    properties.add(DoubleProperty('scale', scale));
  }
}
