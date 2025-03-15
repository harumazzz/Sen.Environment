import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../extension/context.dart';
import '../extension/platform.dart';
import '../i18n/app_localizations.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../cubit/settings_cubit/settings_cubit.dart';
import '../cubit/navigation_cubit/navigation_cubit.dart';
import '../constant/build_distribution.dart';
import 'home_screen/home_screen.dart';
import 'miscellaneous/miscellaneous_screen.dart';
import 'navigation_destination.dart';
import 'setting_screen/setting_screen.dart';
import 'shell_screen/shell_screen.dart';
import '../service/android_helper.dart';
import '../service/ui_helper.dart';
import '../widget/hotkey.dart';

class RootScreen extends StatelessWidget {
  const RootScreen({super.key, required this.bucket});
  final PageStorageBucket bucket;

  static const List<Widget> _destinations = [
    HomeScreen(key: PageStorageKey<String>('home')),
    MiscellaneousScreen(key: PageStorageKey<String>('miscellaneous')),
    SettingScreen(key: PageStorageKey<String>('settings')),
  ];

  void _handleAndroidPermissions(BuildContext context) {
    if (!CurrentPlatform.isAndroid) {
      return;
    }
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
        await settingsCubit.setRequestedPermission(requestedPermission: true);
      }
    });
  }

  Future<void> _showPermissionDialog(BuildContext context) async {
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
    if (!CurrentPlatform.isAndroid || AndroidHelper.arguments == null) {
      return;
    }
    WidgetsBinding.instance.addPostFrameCallback((_) {
      Navigator.of(
        context,
      ).push(MaterialPageRoute(builder: (context) => const ShellScreen()));
    });
  }

  Widget _buildNavigationBar(BuildContext context, NavigationState state) {
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

  Widget _buildTransition(NavigationState state) {
    return Expanded(
      child: AnimatedSwitcher(
        duration: const Duration(milliseconds: 100),
        child: _destinations[state.selectedIndex],
        transitionBuilder:
            (child, animation) =>
                FadeTransition(opacity: animation, child: child),
      ),
    );
  }

  AppBar? _buildAppBar() {
    return CurrentPlatform.isDesktop
        ? null
        : AppBar(
          title: const Text(BuildDistribution.kApplicationName),
          centerTitle: false,
        );
  }

  @override
  Widget build(BuildContext context) {
    _handleAndroidPermissions(context);
    _processAndroidArguments(context);
    return BlocProvider<NavigationCubit>(
      create: (_) => NavigationCubit(),
      child: HotkeyBuilder(
        child: BlocBuilder<NavigationCubit, NavigationState>(
          builder: (context, state) {
            return PageStorage(
              bucket: bucket,
              child: Scaffold(
                appBar: _buildAppBar(),
                body: Row(
                  children: [
                    if (CurrentPlatform.isDesktop)
                      CollapsibleNavigationRail(state: state),
                    _buildTransition(state),
                  ],
                ),
                bottomNavigationBar:
                    CurrentPlatform.isMobile
                        ? _buildNavigationBar(context, state)
                        : null,
              ),
            );
          },
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<PageStorageBucket>('bucket', bucket));
  }
}
