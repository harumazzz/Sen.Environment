import 'package:flutter/material.dart';
import 'package:website/screen/about/about_page.dart';
import 'package:website/screen/changelog/changelog_page.dart';
import 'package:website/screen/download/download_page.dart';
import 'package:website/screen/home/home_page.dart';

class RootPage extends StatefulWidget {
  const RootPage({
    super.key,
  });

  @override
  State<RootPage> createState() => _RootPageState();
}

class _RootPageState extends State<RootPage> {
  late int _selectedIndex;

  late final PageStorageBucket _bucket;

  @override
  void initState() {
    _selectedIndex = 0;
    _bucket = PageStorageBucket();
    super.initState();
  }

  @override
  void dispose() {
    super.dispose();
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  void _onItemTapped(int index) {
    setState(() {
      _selectedIndex = index;
    });
  }

  Widget _buildMenuItem(
    String title,
    int index,
  ) {
    return ListTile(
      title: Text(title),
      selected: _selectedIndex == index,
      onTap: () {
        _onItemTapped(index);
        Navigator.pop(context);
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Sen: Download page'),
      ),
      body: PageStorage(
        bucket: _bucket,
        child: IndexedStack(
          index: _selectedIndex,
          children: <Widget>[
            HomePage(
              key: const PageStorageKey('home'),
              onNavigate: _onItemTapped,
            ),
            DownloadPage(
              key: const PageStorageKey('download'),
              onNavigate: _onItemTapped,
            ),
            ChangelogPage(
              key: const PageStorageKey('changelog'),
              onNavigate: _onItemTapped,
            ),
            AboutPage(
              key: const PageStorageKey('about'),
              onNavigate: _onItemTapped,
            ),
          ],
        ),
      ),
      drawer: Drawer(
        child: ListView(
          padding: EdgeInsets.zero,
          children: [
            DrawerHeader(
              decoration: BoxDecoration(
                color: Theme.of(context).brightness == Brightness.dark ? Colors.grey[850] : Colors.pink[100],
                boxShadow: [
                  BoxShadow(
                    color: Theme.of(context).brightness == Brightness.dark
                        ? Colors.black.withValues(alpha: 0.5)
                        : Colors.grey.withValues(alpha: 0.3),
                    spreadRadius: 1,
                    blurRadius: 8,
                    offset: const Offset(0, 4),
                  ),
                ],
              ),
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Row(
                      children: [
                        ClipRRect(
                          borderRadius: BorderRadius.circular(12.0),
                          child: Image.asset(
                            'assets/images/logo.png',
                            width: 50,
                            height: 50,
                            fit: BoxFit.cover,
                          ),
                        ),
                        const SizedBox(width: 12.0),
                        Text(
                          'Sen: Environment',
                          style: Theme.of(context).textTheme.titleLarge?.copyWith(
                                fontWeight: FontWeight.bold,
                                color: Theme.of(context).brightness == Brightness.dark ? Colors.white : Colors.black,
                                fontSize: 22.0,
                              ),
                        ),
                      ],
                    ),
                    const SizedBox(height: 12.0),
                    Text(
                      'Make your own PvZ2 mod with Sen.',
                      style: Theme.of(context).textTheme.titleLarge?.copyWith(
                            color:
                                Theme.of(context).brightness == Brightness.dark ? Colors.grey[300] : Colors.grey[700],
                            fontSize: 14.0,
                          ),
                    ),
                  ],
                ),
              ),
            ),
            _buildMenuItem('Home', 0),
            _buildMenuItem('Download', 1),
            _buildMenuItem('Changelog', 2),
            _buildMenuItem('About', 3),
          ],
        ),
      ),
    );
  }
}
