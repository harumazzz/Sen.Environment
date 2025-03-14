import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';

@immutable
class ControlButton extends StatelessWidget {
  const ControlButton({
    required this.icon,
    required this.tooltip,
    required this.onPressed,
    super.key,
  });
  final IconData icon;
  final String tooltip;
  final VoidCallback onPressed;

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);

    return Tooltip(
      message: tooltip,
      child: InkWell(
        borderRadius: BorderRadius.circular(16.0),
        onTap: onPressed,
        child: Ink(
          padding: const EdgeInsets.symmetric(vertical: 8.0, horizontal: 8.0),
          decoration: BoxDecoration(
            color: theme.colorScheme.surfaceContainerHighest,
            borderRadius: BorderRadius.circular(12.0),
            boxShadow: [
              BoxShadow(
                color: theme.shadowColor.withValues(alpha: 0.1),
                blurRadius: 4,
                offset: const Offset(0, 2),
              ),
            ],
          ),
          child: Icon(icon, color: theme.colorScheme.onSurfaceVariant),
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<IconData>('icon', icon));
    properties.add(StringProperty('tooltip', tooltip));
    properties.add(
      ObjectFlagProperty<VoidCallback>.has('onPressed', onPressed),
    );
  }
}
