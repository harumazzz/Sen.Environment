import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';

import '../extension/context.dart';

class CollapsibleNavigationRail extends StatefulWidget {
  const CollapsibleNavigationRail({
    super.key,
    required this.selectedIndex,
    required this.onSelect,
  });

  final int selectedIndex;
  final void Function(int index) onSelect;

  @override
  State<CollapsibleNavigationRail> createState() =>
      _CollapsibleNavigationRailState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(ObjectFlagProperty.has('onSelect', onSelect));
    properties.add(IntProperty('selectedIndex', selectedIndex));
  }
}

class _CollapsibleNavigationRailState extends State<CollapsibleNavigationRail>
    with AutomaticKeepAliveClientMixin {
  late bool _isExpanded;

  @override
  void initState() {
    _isExpanded = false;
    super.initState();
  }

  void _toggleNavigationRail() {
    setState(() {
      _isExpanded = !_isExpanded;
    });
  }

  @override
  Widget build(BuildContext context) {
    super.build(context);
    final theme = Theme.of(context);
    return Container(
      margin: const EdgeInsets.all(8.0),
      child: ClipRRect(
        borderRadius: BorderRadius.circular(32.0),
        child: Row(
          children: [
            AnimatedContainer(
              duration: const Duration(milliseconds: 300),
              width: _isExpanded ? 168.0 : 82.0,
              curve: Curves.easeInOut,
              child: Material(
                elevation: 2.0,
                child: NavigationRailTheme(
                  data: NavigationRailThemeData(
                    backgroundColor: theme.colorScheme.surfaceContainerLow,
                    indicatorColor: theme.colorScheme.primaryContainer,
                    indicatorShape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(16.0),
                    ),
                    selectedIconTheme: IconThemeData(
                      color: theme.colorScheme.onPrimaryContainer,
                    ),
                    unselectedIconTheme: IconThemeData(
                      color: theme.colorScheme.onSurfaceVariant,
                    ),
                  ),
                  child: NavigationRail(
                    extended: _isExpanded,
                    selectedIndex: widget.selectedIndex,
                    onDestinationSelected: widget.onSelect,
                    labelType:
                        _isExpanded
                            ? NavigationRailLabelType.none
                            : NavigationRailLabelType.all,
                    useIndicator: true,
                    leading: Padding(
                      padding: const EdgeInsets.symmetric(vertical: 16.0),
                      child: Image.asset(
                        'assets/images/logo.png',
                        height: 48.0,
                      ),
                    ),
                    destinations: _buildDestinations(),
                    trailing: Column(
                      mainAxisAlignment: MainAxisAlignment.end,
                      children: [
                        const SizedBox(height: 8.0),
                        _buildToggleButton(theme),
                        const SizedBox(height: 16.0),
                      ],
                    ),
                  ),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  List<NavigationRailDestination> _buildDestinations() {
    return [
      _buildDestination(Symbols.home, Symbols.home_filled, context.los.home),
      _buildDestination(
        Symbols.package,
        Symbols.package_sharp,
        context.los.task,
      ),
      _buildDestination(
        Symbols.settings,
        Symbols.settings_sharp,
        context.los.settings,
      ),
    ];
  }

  NavigationRailDestination _buildDestination(
    IconData icon,
    IconData selectedIcon,
    String title,
  ) {
    return NavigationRailDestination(
      padding: const EdgeInsets.symmetric(vertical: 8.0),
      icon: Icon(icon, size: 28.0),
      selectedIcon: Icon(selectedIcon, size: 28.0),
      label: AnimatedCrossFade(
        duration: const Duration(milliseconds: 200),
        crossFadeState:
            _isExpanded ? CrossFadeState.showFirst : CrossFadeState.showSecond,
        firstChild: Text(title, style: const TextStyle(fontSize: 14.0)),
        secondChild: const SizedBox.shrink(),
      ),
    );
  }

  Widget _buildToggleButton(ThemeData theme) {
    return IconButton.filledTonal(
      icon: Icon(_isExpanded ? Icons.chevron_left : Icons.chevron_right),
      color: theme.colorScheme.onSurfaceVariant,
      onPressed: _toggleNavigationRail,
    );
  }

  @override
  bool get wantKeepAlive => true;
}
