import 'package:flutter/material.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/screen/animation_viewer/control_button.dart';
import 'package:sen/screen/animation_viewer/visual_helper.dart';

@immutable
class ControlPanel extends StatelessWidget {
  const ControlPanel({
    super.key,
    required this.animationController,
    required this.isPause,
    required this.toggleEvent,
    required this.setState,
    required this.scaleNotifier,
    required this.forcePlay,
    required this.resetFocus,
    required this.visualHelper,
  });

  final bool Function() toggleEvent;

  final VisualHelper visualHelper;

  final void Function(VoidCallback callback) setState;

  final AnimationController animationController;

  final ValueNotifier<double> scaleNotifier;

  final bool isPause;

  final void Function() forcePlay;

  final void Function() resetFocus;

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Card(
      margin: const EdgeInsets.only(bottom: 16.0, left: 16.0, right: 16.0),
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(12),
      ),
      elevation: 4,
      child: Padding(
        padding: const EdgeInsets.all(12.0),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.center,
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
          ],
        ),
      ),
    );
  }
}
