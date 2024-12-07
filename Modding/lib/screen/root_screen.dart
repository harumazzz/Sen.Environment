import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:page_transition/page_transition.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/home/home_screen.dart';
import 'package:sen/screen/miscellaneous/miscellaenous_screen.dart';
import 'package:sen/screen/setting/setting_screen.dart';
import 'package:sen/screen/shell/shell_screen.dart';
import 'package:sen/service/android_service.dart';

class RootScreen extends ConsumerStatefulWidget {
  const RootScreen({super.key, required this.title});

  final String title;

  @override
  ConsumerState<RootScreen> createState() => _RootScreenState();
}

class _RootScreenState extends ConsumerState<RootScreen> {
  int _currentPageIndex = 0;

  final _labelBehavior = NavigationDestinationLabelBehavior.alwaysShow;

  bool _hasNavigated = false;

  final List<Widget> _destinations = const [
    HomeScreen(),
    MiscellaenousScreen(),
    SettingScreen(),
  ];

  @override
  void initState() {
    super.initState();
  }

  void _changeScreen(int index) {
    setState(() {
      _currentPageIndex = index;
    });
  }

  Widget? _makeNavigationBar() {
    final los = AppLocalizations.of(context)!;
    if (Platform.isAndroid || Platform.isIOS) {
      return NavigationBar(
        labelBehavior: _labelBehavior,
        selectedIndex: _currentPageIndex,
        onDestinationSelected: _changeScreen,
        destinations: <Widget>[
          NavigationDestination(
            icon: const Icon(Icons.home_outlined),
            selectedIcon: const Icon(Icons.home_filled),
            label: los.home,
          ),
          NavigationDestination(
            icon: const Icon(Icons.miscellaneous_services_outlined),
            selectedIcon: const Icon(Icons.miscellaneous_services_sharp),
            label: los.miscellaneous,
          ),
          NavigationDestination(
            icon: const Icon(Icons.settings_outlined),
            selectedIcon: const Icon(Icons.settings),
            label: los.settings,
          ),
        ],
      );
    }
    return null;
  }

  Widget _makeNavigationRail() {
    final los = AppLocalizations.of(context)!;
    if (Platform.isWindows || Platform.isLinux || Platform.isMacOS) {
      return NavigationRail(
        selectedIndex: _currentPageIndex,
        onDestinationSelected: (index) {
          setState(() {
            _currentPageIndex = index;
          });
        },
        labelType: NavigationRailLabelType.all,
        destinations: [
          NavigationRailDestination(
            icon: const Icon(Icons.home_outlined),
            selectedIcon: const Icon(Icons.home_filled),
            label: Text(los.home),
          ),
          NavigationRailDestination(
            icon: const Icon(Icons.miscellaneous_services_outlined),
            selectedIcon: const Icon(Icons.miscellaneous_services_sharp),
            label: Text(los.miscellaneous),
          ),
          NavigationRailDestination(
            icon: const Icon(Icons.settings_outlined),
            selectedIcon: const Icon(Icons.settings),
            label: Text(los.settings),
          ),
        ],
      );
    }
    return const SizedBox.shrink();
  }

  Future<void> _displayAllowDialog() async {
    final los = AppLocalizations.of(context)!;
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.android_request),
        content: Text(los.android_storage_access_permission_required),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop();
            },
            child: Text(los.go_to_settings),
          ),
        ],
      ),
    );
  }

  void _requestAndroidPermissionFirstTime() {
    if (!Platform.isAndroid) return;
    Future.sync(
      () async {
        final provider = ref.read(settingProvider);
        if (!provider.requestedPermission) {
          if (!(await AndroidService.checkStoragePermission())) {
            await _displayAllowDialog();
            await AndroidService.requestStoragePermission();
          }
        }
        ref.watch(settingProvider.notifier).setRequestedPermission(true);
      },
    );
  }

  void _loadArgumentOnAndroid() {
    if (!Platform.isAndroid) return;
    if (AndroidService.arguments != null) {
      _hasNavigated = true;
      WidgetsBinding.instance.addPostFrameCallback(
        (_) {
          Navigator.of(context).push(
            PageTransition(
              child: ShellScreen(
                arguments: AndroidService.arguments!,
              ),
              type: PageTransitionType.fade,
              duration: const Duration(
                milliseconds: 300,
              ),
            ),
          );
        },
      );
      _hasNavigated = false;
    }
  }

  @override
  Widget build(BuildContext context) {
    _requestAndroidPermissionFirstTime();
    if (!_hasNavigated) {
      _loadArgumentOnAndroid();
    }
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
      ),
      body: Row(
        children: [
          _makeNavigationRail(),
          Expanded(
            child: AnimatedSwitcher(
              duration: const Duration(
                milliseconds: 100,
              ),
              child: _destinations[_currentPageIndex],
              transitionBuilder: (child, animation) {
                return FadeTransition(
                  opacity: animation,
                  child: child,
                );
              },
            ),
          ),
        ],
      ),
      bottomNavigationBar: _makeNavigationBar(),
    );
  }
}
