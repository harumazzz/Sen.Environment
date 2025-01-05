import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:page_transition/page_transition.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/screen/home/home_screen.dart';
import 'package:sen/screen/miscellaneous/miscellaenous_screen.dart';
import 'package:sen/screen/setting/setting_screen.dart';
import 'package:sen/screen/shell/shell_screen.dart';
import 'package:sen/service/android_helper.dart';

class RootScreen extends StatefulWidget {
  const RootScreen({super.key, required this.title});

  final String title;

  @override
  State<RootScreen> createState() => _RootScreenState();
}

class _RootScreenState extends State<RootScreen> {
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
            icon: const Icon(Symbols.home),
            selectedIcon: const Icon(Symbols.home_filled),
            label: los.home,
          ),
          NavigationDestination(
            icon: const Icon(Symbols.package),
            selectedIcon: const Icon(Symbols.package_sharp),
            label: los.miscellaneous,
          ),
          NavigationDestination(
            icon: const Icon(Symbols.settings),
            selectedIcon: const Icon(Symbols.settings_sharp),
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
            icon: const Icon(Symbols.home),
            selectedIcon: const Icon(Symbols.home_filled),
            label: Text(los.home),
          ),
          NavigationRailDestination(
            icon: const Icon(Symbols.package),
            selectedIcon: const Icon(Symbols.package_sharp),
            label: Text(los.miscellaneous),
          ),
          NavigationRailDestination(
            icon: const Icon(Symbols.settings),
            selectedIcon: const Icon(Symbols.settings_sharp),
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
        void setPermission() {
          BlocProvider.of<SettingsCubit>(context).setRequestedPermission(true);
        }

        final provider = BlocProvider.of<SettingsCubit>(context).state;
        if (!provider.requestedPermission) {
          if (!(await AndroidHelper.checkStoragePermission())) {
            await _displayAllowDialog();
            await AndroidHelper.requestStoragePermission();
          }
        }
        setPermission();
      },
    );
  }

  void _loadArgumentOnAndroid() {
    if (!Platform.isAndroid) return;
    if (AndroidHelper.arguments != null) {
      _hasNavigated = true;
      WidgetsBinding.instance.addPostFrameCallback(
        (_) {
          Navigator.of(context).push(
            PageTransition(
              child: ShellScreen(
                arguments: AndroidHelper.arguments!,
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
