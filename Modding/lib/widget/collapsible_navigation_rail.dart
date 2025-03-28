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
                    destinations: [
                      NavigationRailDestination(
                        padding: const EdgeInsets.symmetric(vertical: 8.0),
                        icon: const Icon(Symbols.home),
                        selectedIcon: const Icon(Symbols.home_filled),
                        label: CustomRailLabel(
                          isExpanded: _isExpanded,
                          title: context.los.home,
                          isSelected: widget.selectedIndex == 0,
                        ),
                      ),
                      NavigationRailDestination(
                        padding: const EdgeInsets.symmetric(vertical: 8.0),
                        icon: const Icon(Symbols.package),
                        selectedIcon: const Icon(Symbols.package_sharp),
                        label: CustomRailLabel(
                          isExpanded: _isExpanded,
                          title: context.los.task,
                          isSelected: widget.selectedIndex == 1,
                        ),
                      ),
                      NavigationRailDestination(
                        padding: const EdgeInsets.symmetric(vertical: 8.0),
                        icon: const Icon(Symbols.settings),
                        selectedIcon: const Icon(Symbols.settings_sharp),
                        label: CustomRailLabel(
                          isExpanded: _isExpanded,
                          title: context.los.settings,
                          isSelected: widget.selectedIndex == 2,
                        ),
                      ),
                    ],
                    trailing: Column(
                      mainAxisAlignment: MainAxisAlignment.end,
                      children: [
                        const SizedBox(height: 8.0),
                        CollapsibleNavigationRailButton(
                          onPressed: _toggleNavigationRail,
                          isExpanded: _isExpanded,
                        ),
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

  @override
  bool get wantKeepAlive => true;
}

class CustomRailLabel extends StatelessWidget {
  const CustomRailLabel({
    super.key,
    required this.isExpanded,
    required this.title,
    required this.isSelected,
  });

  final bool isExpanded;

  final String title;

  final bool isSelected;

  @override
  Widget build(BuildContext context) {
    return AnimatedCrossFade(
      duration: const Duration(milliseconds: 200),
      crossFadeState:
          isExpanded ? CrossFadeState.showFirst : CrossFadeState.showSecond,
      firstChild: Text(
        title,
        style: TextStyle(
          fontSize: 14.0,
          fontWeight: isSelected ? FontWeight.bold : FontWeight.normal,
        ),
      ),
      secondChild: const SizedBox.shrink(),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<bool>('isExpanded', isExpanded));
    properties.add(StringProperty('title', title));
    properties.add(DiagnosticsProperty<bool>('isSelected', isSelected));
  }
}

class CollapsibleNavigationRailButton extends StatelessWidget {
  const CollapsibleNavigationRailButton({
    super.key,
    required this.onPressed,
    required this.isExpanded,
  });

  final void Function() onPressed;
  final bool isExpanded;

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);
    return IconButton.filledTonal(
      icon: Icon(isExpanded ? Icons.chevron_left : Icons.chevron_right),
      color: theme.colorScheme.onSurfaceVariant,
      onPressed: onPressed,
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<void Function()>.has('onPressed', onPressed),
    );
    properties.add(DiagnosticsProperty<bool>('isExpanded', isExpanded));
  }
}
