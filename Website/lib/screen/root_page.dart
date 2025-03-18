import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';
import '../extension/context.dart';

class RootPage extends StatelessWidget {
  const RootPage({
    super.key,
    required this.child,
    required this.selectedIndex,
    required this.onNavigate,
  });
  final Widget child;

  final int selectedIndex;

  final void Function(int index) onNavigate;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text(context.los.download_page)),
      body: child,
      drawer: AppDrawer(selectedIndex: selectedIndex, onNavigate: onNavigate),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(IntProperty('selectedIndex', selectedIndex));
    properties.add(
      ObjectFlagProperty<void Function(int index)>.has(
        'onNavigate',
        onNavigate,
      ),
    );
  }
}

class AppDrawer extends StatelessWidget {
  const AppDrawer({
    super.key,
    required this.onNavigate,
    required this.selectedIndex,
  });

  final void Function(int index) onNavigate;
  final int selectedIndex;

  @override
  Widget build(BuildContext context) {
    return Drawer(
      child: ListView(
        padding: EdgeInsets.zero,
        children: [
          _buildDrawerHeader(context),
          _buildMenuItem(context.los.home, 0, context, '/'),
          _buildMenuItem(context.los.download, 1, context, '/download'),
          _buildMenuItem(context.los.changelog, 2, context, '/changelog'),
          _buildMenuItem(context.los.about, 3, context, '/about'),
          _buildMenuItem(context.los.extension, 4, context, '/extension'),
        ],
      ),
    );
  }

  Widget _buildDrawerHeader(BuildContext context) {
    final isDarkMode = Theme.of(context).brightness == Brightness.dark;
    return DrawerHeader(
      decoration: BoxDecoration(
        gradient: LinearGradient(
          colors:
              isDarkMode
                  ? const [Color(0xFF393E46), Color(0xFF222831)]
                  : const [Color(0xFFFFE4E1), Color(0xFFFFB6C1)],
          begin: Alignment.topLeft,
          end: Alignment.bottomRight,
        ),
      ),
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        spacing: 10.0,
        children: [
          Image.asset('assets/images/logo.png', height: 80, width: 80),
          const Text('Sen: Environment', style: TextStyle(fontSize: 22.0)),
        ],
      ),
    );
  }

  Widget _buildMenuItem(
    String title,
    int index,
    BuildContext context,
    String route,
  ) {
    return ListTile(
      title: Text(title),
      selected: selectedIndex == index,
      onTap: () {
        onNavigate(index);
        context.go(route);
        Navigator.of(context).pop();
      },
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<void Function(int index)>.has(
        'onNavigate',
        onNavigate,
      ),
    );
    properties.add(IntProperty('selectedIndex', selectedIndex));
  }
}
