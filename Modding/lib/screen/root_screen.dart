import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:page_transition/page_transition.dart';
import 'package:sen/model/firebase_message.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/repository/message_repository.dart';
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
        actions: [
          NotificationWidget(
            messageRepository: MessageRepository(),
            los: AppLocalizations.of(context)!,
          ),
        ],
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

class NotificationWidget extends StatelessWidget {
  final MessageRepository messageRepository;
  final AppLocalizations los;

  const NotificationWidget({
    super.key,
    required this.messageRepository,
    required this.los,
  });

  String _monthName(int month) {
    const months = [
      'Jan',
      'Feb',
      'Mar',
      'Apr',
      'May',
      'Jun',
      'Jul',
      'Aug',
      'Sep',
      'Oct',
      'Nov',
      'Dec'
    ];
    return months[month - 1];
  }

  String _formatTimestamp(DateTime timestamp) {
    final now = DateTime.now();
    final difference = now.difference(timestamp);

    if (difference.inMinutes < 1) {
      return 'Just now';
    } else if (difference.inMinutes < 60) {
      return '${difference.inMinutes} mins ago';
    } else if (difference.inHours < 24) {
      return '${difference.inHours} hours ago';
    } else {
      return '${timestamp.day} ${_monthName(timestamp.month)} ${timestamp.year}, '
          '${timestamp.hour}:${timestamp.minute.toString().padLeft(2, '0')}';
    }
  }

  Widget _buildNotificationCard(
    BuildContext context,
    FirebaseMessage data,
  ) {
    return Card(
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(12),
      ),
      child: ListTile(
        leading: Icon(
          Icons.notifications,
          color: Colors.blue.shade700,
        ),
        title: Text(
          data.title,
          style: const TextStyle(
            fontWeight: FontWeight.bold,
          ),
        ),
        subtitle: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          mainAxisAlignment: MainAxisAlignment.start,
          children: [
            const SizedBox(height: 6.0),
            Text(
              data.description,
              style: const TextStyle(fontSize: 14),
              maxLines: 10,
              overflow: TextOverflow.ellipsis,
            ),
            const SizedBox(height: 4.0),
            Text(
              _formatTimestamp(data.createdAt),
              style: Theme.of(context).textTheme.labelSmall,
            ),
            const SizedBox(height: 4.0),
          ],
        ),
      ),
    );
  }

  Widget _buildNotificationList(
    BuildContext context,
    List<FirebaseMessage> messages,
  ) {
    return SizedBox(
      width: double.maxFinite,
      child: SingleChildScrollView(
        child: ConstrainedBox(
          constraints: const BoxConstraints(maxHeight: 400.0),
          child: ListView.builder(
            shrinkWrap: true,
            itemCount: messages.length,
            itemBuilder: (context, index) =>
                _buildNotificationCard(context, messages[index]),
          ),
        ),
      ),
    );
  }

  Future<void> _showNotifications(
    BuildContext context,
  ) async {
    await showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(los.notification),
        titlePadding: const EdgeInsets.fromLTRB(16, 16, 16, 0),
        content: StreamBuilder<List<FirebaseMessage>>(
          stream: messageRepository.getMessage(),
          builder: (context, snapshot) {
            if (snapshot.connectionState == ConnectionState.waiting) {
              return const Center(child: CircularProgressIndicator.adaptive());
            } else if (snapshot.hasError) {
              return Text(snapshot.error.toString());
            } else if (snapshot.hasData && snapshot.data!.isNotEmpty) {
              return _buildNotificationList(context, snapshot.data!);
            } else {
              return const Center(
                child: Text(
                  "No notifications available",
                  style: TextStyle(fontSize: 16, color: Colors.black54),
                ),
              );
            }
          },
        ),
        contentPadding: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(16),
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.of(context).pop(),
            child: Text(
              los.okay,
              style: TextStyle(color: Colors.blue.shade700),
            ),
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Tooltip(
      message: los.notification,
      child: IconButton(
        onPressed: () => _showNotifications(context),
        icon: const Icon(Symbols.notification_add),
      ),
    );
  }
}
