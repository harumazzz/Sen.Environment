import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import '../constant/system_overlay.dart';
import '../extension/context.dart';
import '../extension/platform.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../constant/build_distribution.dart';
import '../service/ui_helper.dart';
import 'home_screen/home_screen.dart';
import 'miscellaneous/miscellaneous_screen.dart';
import '../widget/collapsible_navigation_rail.dart';
import 'setting_screen/setting_screen.dart';
import '../widget/hotkey.dart';

class RootScreen extends StatefulWidget {
  const RootScreen({super.key});

  @override
  State<RootScreen> createState() => _RootScreenState();
}

class _RootScreenState extends State<RootScreen> {
  int _selectedIndex = 0;

  void _onDestinationSelected(int index) {
    setState(() {
      _selectedIndex = index;
    });
  }

  Widget _buildNavigationBar(BuildContext context) {
    final los = context.los;
    return NavigationBar(
      labelBehavior: NavigationDestinationLabelBehavior.alwaysShow,
      selectedIndex: _selectedIndex,
      onDestinationSelected: _onDestinationSelected,
      destinations: [
        NavigationDestination(
          icon: const Icon(Symbols.home),
          selectedIcon: const Icon(Symbols.home_filled),
          label: los.home,
        ),
        NavigationDestination(
          icon: const Icon(Symbols.package),
          selectedIcon: const Icon(Symbols.package_sharp),
          label: los.task,
        ),
        NavigationDestination(
          icon: const Icon(Symbols.settings),
          selectedIcon: const Icon(Symbols.settings_sharp),
          label: los.settings,
        ),
      ],
    );
  }

  Widget? _buildBottomNavigationBar(BuildContext context) {
    return CurrentPlatform.isMobile ? _buildNavigationBar(context) : null;
  }

  @override
  Widget build(BuildContext context) {
    SystemOverlay.apply(Theme.of(context).brightness);
    return HotkeyBuilder(
      child: Scaffold(
        body: UIHelper.applyScrollView(
          builder: (context, innerBoxIsScrolled) {
            return UIHelper.buildAppBar(
              context,
              innerBoxIsScrolled,
              title: const Text(
                BuildDistribution.kApplicationName,
                style: TextStyle(fontWeight: FontWeight.bold),
              ),
            );
          },
          child: Row(
            children: [
              if (CurrentPlatform.isDesktop)
                CollapsibleNavigationRail(
                  selectedIndex: _selectedIndex,
                  onSelect: _onDestinationSelected,
                ),
              Expanded(
                child: IndexedStack(
                  index: _selectedIndex,
                  children: [
                    const HomeScreen(),
                    const MiscellaneousScreen(),
                    if (_selectedIndex == 2) const SettingScreen(),
                  ],
                ),
              ),
            ],
          ),
        ),
        bottomNavigationBar: _buildBottomNavigationBar(context),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(IntProperty('selectedIndex', _selectedIndex));
  }
}
