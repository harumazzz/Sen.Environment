import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../bloc/selected_image_bloc/selected_image_bloc.dart';
import '../../bloc/selected_sprite_bloc/selected_sprite_bloc.dart';
import '../../extension/context.dart';
import '../../i18n/app_localizations.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'control_button.dart';
import 'visual_helper.dart';

@immutable
class ControlPanel extends StatelessWidget {
  const ControlPanel({
    super.key,
    required this.animationController,
    required this.isPause,
    required this.toggleEvent,
    required this.setState,
    required this.forcePlay,
    required this.resetFocus,
    required this.visualHelper,
    required this.popupMenuKey,
  });

  final bool Function() toggleEvent;

  final VisualHelper visualHelper;

  final void Function(VoidCallback callback) setState;

  final AnimationController animationController;

  final bool isPause;

  final void Function() forcePlay;

  final void Function() resetFocus;

  final GlobalKey<PopupMenuButtonState<String>> popupMenuKey;

  void _onAdd(BuildContext context, String value) {
    switch (value) {
      case 'enable_sprites':
        context.read<SelectedSpriteBloc>().add(
          const SelectedSpriteEnableAllEvent(),
        );
        break;
      case 'disable_sprites':
        context.read<SelectedSpriteBloc>().add(
          const SelectedSpriteDisableAllEvent(),
        );
        break;
      case 'enable_images':
        context.read<SelectedImageBloc>().add(
          const SelectedImageEnableAllEvent(),
        );
        break;
      case 'disable_images':
        context.read<SelectedImageBloc>().add(
          const SelectedImageDisableAllEvent(),
        );
        break;
    }
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Card(
      margin: const EdgeInsets.symmetric(vertical: 4.0, horizontal: 16.0),
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(20.0)),
      color: Theme.of(context).colorScheme.surfaceContainer,
      elevation: 4.0,
      child: Padding(
        padding: const EdgeInsets.all(8.0),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.center,
          spacing: 10.0,
          children: [
            ControlButton(
              icon: Symbols.reset_focus,
              tooltip: los.reset_focus,
              onPressed: resetFocus,
            ),
            ControlButton(
              icon: Symbols.arrow_back,
              tooltip: los.backward,
              onPressed: () {
                visualHelper.workingFrameRate /= 2;
                if (visualHelper.workingFrameRate <= 0) {
                  visualHelper.workingFrameRate = 1;
                }
                setState(() {});
              },
            ),
            ControlButton(
              icon: !isPause ? Symbols.pause : Symbols.play_arrow,
              tooltip: isPause ? los.play : los.stop,
              onPressed: () {
                setState(() {
                  final isPause = toggleEvent();
                  if (isPause) {
                    animationController.stop();
                    return;
                  }
                  if (animationController.duration != null) {
                    animationController.forward();
                  }
                });
              },
            ),
            ControlButton(
              icon: Symbols.arrow_forward,
              tooltip: los.forward,
              onPressed: () {
                visualHelper.workingFrameRate *= 2;
                if (visualHelper.workingFrameRate > 240) {
                  visualHelper.workingFrameRate = 240;
                }
                setState(() {});
              },
            ),
            ControlButton(
              icon: Symbols.repeat,
              tooltip: los.restart,
              onPressed: () {
                if (animationController.duration != null) {
                  animationController.forward(from: 0.0);
                }
                forcePlay();
              },
            ),
            PopupMenuButton<String>(
              key: popupMenuKey,
              tooltip: context.los.add,
              itemBuilder:
                  (context) => [
                    PopupMenuItem(
                      value: 'disable_sprites',
                      child: Text(context.los.disable_all_sprite),
                    ),
                    PopupMenuItem(
                      value: 'enable_sprites',
                      child: Text(context.los.enable_all_sprite),
                    ),
                    PopupMenuItem(
                      value: 'disable_images',
                      child: Text(context.los.disable_all_image),
                    ),
                    PopupMenuItem(
                      value: 'enable_images',
                      child: Text(context.los.enable_all_image),
                    ),
                  ],
              onSelected: (value) => _onAdd(context, value),
              child: ControlButton(
                icon: Symbols.add,
                tooltip: los.add,
                onPressed: () {
                  popupMenuKey.currentState?.showButtonMenu();
                },
              ),
            ),
          ],
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<bool Function()>.has('toggleEvent', toggleEvent),
    );
    properties.add(
      DiagnosticsProperty<VisualHelper>('visualHelper', visualHelper),
    );
    properties.add(
      ObjectFlagProperty<void Function(VoidCallback callback)>.has(
        'setState',
        setState,
      ),
    );
    properties.add(
      DiagnosticsProperty<AnimationController>(
        'animationController',
        animationController,
      ),
    );
    properties.add(DiagnosticsProperty<bool>('isPause', isPause));
    properties.add(
      ObjectFlagProperty<void Function()>.has('forcePlay', forcePlay),
    );
    properties.add(
      ObjectFlagProperty<void Function()>.has('resetFocus', resetFocus),
    );
    properties.add(
      DiagnosticsProperty<GlobalKey<PopupMenuButtonState<String>>>(
        'popupMenuKey',
        popupMenuKey,
      ),
    );
  }
}
