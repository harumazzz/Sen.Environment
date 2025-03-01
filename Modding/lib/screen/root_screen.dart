import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/extension/context.dart';
import 'package:sen/extension/platform.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/cubit/navigation_cubit/navigation_cubit.dart';
import 'package:sen/constant/build_distribution.dart';
import 'package:sen/screen/home_screen/home_screen.dart';
import 'package:sen/screen/miscellaneous/miscellaneous_screen.dart';
import 'package:sen/screen/setting_screen/setting_screen.dart';
import 'package:sen/screen/shell_screen/shell_screen.dart';
import 'package:sen/service/android_helper.dart';
import 'package:sen/service/ui_helper.dart';
import 'package:sen/widget/hotkey.dart';

class RootScreen extends StatelessWidget {
  const RootScreen({super.key});

  static const List<Widget> _destinations = [
    HomeScreen(),
    MiscellaneousScreen(),
    SettingScreen(),
  ];

  void _handleAndroidPermissions(BuildContext context) {
    if (!CurrentPlatform.isAndroid) return;
    final settingsCubit = context.read<SettingsCubit>();
    Future<void> showAndroidPermission() async {
      await _showPermissionDialog(context);
    }

    Future.microtask(() async {
      if (!settingsCubit.state.requestedPermission) {
        if (!(await AndroidHelper.checkStoragePermission())) {
          await showAndroidPermission();
          await AndroidHelper.requestStoragePermission();
        }
        settingsCubit.setRequestedPermission(true);
      }
    });
  }

  Future<void> _showPermissionDialog(
    BuildContext context,
  ) async {
    final los = AppLocalizations.of(context)!;
    await UIHelper.showFlutterDialog(
      context: context,
      child: UIHelper.buildDialog(
        title: Text(los.android_request),
        content: Text(los.android_storage_access_permission_required),
        actions: [
          TextButton(
            onPressed: () => Navigator.of(context).pop(),
            child: Text(los.go_to_settings),
          ),
          ...UIHelper.buildSimpleAction(context: context),
        ],
      ),
    );
  }

  void _processAndroidArguments(BuildContext context) {
    if (!CurrentPlatform.isAndroid || AndroidHelper.arguments == null) return;
    WidgetsBinding.instance.addPostFrameCallback((_) {
      Navigator.of(context).push(
        MaterialPageRoute(builder: (context) => const ShellScreen()),
      );
    });
  }

  List<Widget> _buildNavigationRail(
    BuildContext context,
    NavigationState state,
  ) {
    final los = context.los;
    final theme = Theme.of(context);
    return [
      Container(
        margin: const EdgeInsets.all(8.0),
        decoration: BoxDecoration(
          color: theme.colorScheme.surfaceContainerHighest,
          borderRadius: BorderRadius.circular(32),
          boxShadow: [
            BoxShadow(
              color: theme.shadowColor.withValues(alpha: 0.1),
              blurRadius: 12,
              spreadRadius: 3,
            ),
          ],
        ),
        child: ClipRRect(
          borderRadius: BorderRadius.circular(32.0),
          child: NavigationRailTheme(
            data: NavigationRailThemeData(
              backgroundColor: Colors.transparent,
              indicatorColor: theme.colorScheme.primaryContainer,
              indicatorShape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(16),
              ),
              selectedIconTheme: IconThemeData(
                color: theme.colorScheme.onPrimaryContainer,
              ),
              selectedLabelTextStyle: TextStyle(
                color: theme.colorScheme.onPrimaryContainer,
              ),
              unselectedIconTheme: IconThemeData(
                color: theme.colorScheme.onSurfaceVariant,
              ),
              unselectedLabelTextStyle: TextStyle(
                color: theme.colorScheme.onSurfaceVariant,
              ),
            ),
            child: NavigationRail(
              selectedIndex: state.selectedIndex,
              onDestinationSelected: context.read<NavigationCubit>().changeIndex,
              labelType: NavigationRailLabelType.all,
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
                  label: los.home,
                ),
                _buildDestination(
                  context,
                  icon: Symbols.package,
                  selectedIcon: Symbols.package_sharp,
                  label: los.task,
                ),
                _buildDestination(
                  context,
                  icon: Symbols.settings,
                  selectedIcon: Symbols.settings_sharp,
                  label: los.settings,
                ),
              ],
            ),
          ),
        ),
      ),
    ];
  }

  NavigationRailDestination _buildDestination(
    BuildContext context, {
    required IconData icon,
    required IconData selectedIcon,
    required String label,
  }) {
    return NavigationRailDestination(
      padding: const EdgeInsets.symmetric(
        vertical: 8.0,
        horizontal: 6.0,
      ),
      icon: Icon(icon, size: 24.0),
      selectedIcon: Icon(selectedIcon, size: 24.0),
      label: Text(
        label,
        overflow: TextOverflow.ellipsis,
      ),
    );
  }

  Widget _buildNavigationBar(
    BuildContext context,
    NavigationState state,
  ) {
    final los = context.los;
    return NavigationBar(
      labelBehavior: NavigationDestinationLabelBehavior.alwaysShow,
      selectedIndex: state.selectedIndex,
      onDestinationSelected: context.read<NavigationCubit>().changeIndex,
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

  Widget _buildTransition(
    NavigationState state,
  ) {
    return Expanded(
      child: AnimatedSwitcher(
        duration: const Duration(milliseconds: 100),
        child: _destinations[state.selectedIndex],
        transitionBuilder: (child, animation) => FadeTransition(
          opacity: animation,
          child: child,
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    _handleAndroidPermissions(context);
    _processAndroidArguments(context);

    return BlocProvider(
      create: (_) => NavigationCubit(),
      child: HotkeyBuilder(
        child: BlocBuilder<NavigationCubit, NavigationState>(
          builder: (context, state) {
            return Scaffold(
              appBar: AppBar(
                forceMaterialTransparency: CurrentPlatform.isDesktop,
                title: const Text(BuildDistribution.kApplicationName),
                centerTitle: false,
              ),
              body: Row(
                children: [
                  if (CurrentPlatform.isDesktop) ..._buildNavigationRail(context, state),
                  _buildTransition(state),
                ],
              ),
              bottomNavigationBar: CurrentPlatform.isMobile ? _buildNavigationBar(context, state) : null,
            );
          },
        ),
      ),
    );
  }
}
