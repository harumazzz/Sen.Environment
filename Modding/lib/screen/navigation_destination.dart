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
    properties.add(
      ObjectFlagProperty<void Function(int index)>.has('onSelect', onSelect),
    );
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
              width: _isExpanded ? 168 : 82,
              curve: Curves.easeInOut,
              child: Material(
                elevation: 2,
                child: NavigationRailTheme(
                  data: NavigationRailThemeData(
                    backgroundColor: theme.colorScheme.surfaceContainerLow,
                    indicatorColor: theme.colorScheme.primaryContainer,
                    indicatorShape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(16),
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
                      padding: const EdgeInsets.symmetric(vertical: 16),
                      child: Image.asset('assets/images/logo.png', height: 48),
                    ),
                    destinations: [
                      _buildDestination(
                        context,
                        icon: Symbols.home,
                        selectedIcon: Symbols.home_filled,
                        title: context.los.home,
                      ),
                      _buildDestination(
                        context,
                        icon: Symbols.package,
                        selectedIcon: Symbols.package_sharp,
                        title: context.los.task,
                      ),
                      _buildDestination(
                        context,
                        icon: Symbols.settings,
                        selectedIcon: Symbols.settings_sharp,
                        title: context.los.settings,
                      ),
                    ],
                    trailing: Column(
                      mainAxisAlignment: MainAxisAlignment.end,
                      children: [
                        const SizedBox(height: 8),
                        _buildToggleButton(theme),
                        const SizedBox(height: 16),
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

  Widget _buildToggleButton(ThemeData theme) {
    return IconButton.filledTonal(
      icon: Icon(_isExpanded ? Icons.chevron_left : Icons.chevron_right),
      color: theme.colorScheme.onSurfaceVariant,
      onPressed: _toggleNavigationRail,
    );
  }

  NavigationRailDestination _buildDestination(
    BuildContext context, {
    required IconData icon,
    required IconData selectedIcon,
    required String title,
  }) {
    return NavigationRailDestination(
      padding: const EdgeInsets.symmetric(vertical: 8.0),
      icon: Icon(icon, size: 28),
      selectedIcon: Icon(selectedIcon, size: 28),
      label: AnimatedCrossFade(
        duration: const Duration(milliseconds: 200),
        crossFadeState:
            _isExpanded ? CrossFadeState.showFirst : CrossFadeState.showSecond,
        firstChild: Text(title, style: const TextStyle(fontSize: 14)),
        secondChild: const SizedBox.shrink(),
      ),
    );
  }

  @override
  bool get wantKeepAlive => true;
}
