import 'package:flutter/material.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import 'package:website/screen/footer/footer_widget.dart';

class AboutPage extends StatelessWidget {
  const AboutPage({
    super.key,
    required this.onNavigate,
  });

  final void Function(int index) onNavigate;

  @override
  Widget build(BuildContext context) {
    final isDarkTheme = Theme.of(context).brightness == Brightness.dark;
    return SingleChildScrollView(
      scrollDirection: Axis.vertical,
      child: Column(
        children: [
          Padding(
            padding: const EdgeInsets.all(24.0),
            child: Column(
              children: [
                Container(
                  width: double.infinity,
                  padding: const EdgeInsets.symmetric(vertical: 40.0, horizontal: 24.0),
                  decoration: BoxDecoration(
                    gradient: LinearGradient(
                      colors: Theme.of(context).brightness == Brightness.light
                          ? [Colors.blueAccent, Colors.lightBlue.shade100]
                          : [Colors.blueAccent, Colors.pinkAccent.shade100],
                      begin: Alignment.topLeft,
                      end: Alignment.bottomRight,
                    ),
                    borderRadius: BorderRadius.circular(16.0),
                  ),
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Image.asset(
                        'assets/images/logo.png',
                        width: 120,
                        height: 120,
                        fit: BoxFit.contain,
                      ),
                      const SizedBox(height: 20.0),
                      const Text(
                        'Sen.Environment',
                        textAlign: TextAlign.center,
                        style: TextStyle(
                          fontSize: 36.0,
                          fontWeight: FontWeight.bold,
                          color: Colors.white,
                        ),
                      ),
                      const SizedBox(height: 8.0),
                      const Text(
                        'An open-source project created by Haruma.',
                        textAlign: TextAlign.center,
                        style: TextStyle(
                          fontSize: 18.0,
                          color: Colors.white70,
                        ),
                      ),
                    ],
                  ),
                ),
                const SizedBox(height: 24.0),
                _infoCard(
                  icon: Symbols.info,
                  title: 'About',
                  content: const Text(
                      'Sen.Environment is a project designed to summarize my personal expertise in C++ & TypeScript, offering cross-platform capabilities and a robust modular architecture.'),
                ),
                _infoCard(
                  icon: Symbols.open_in_new,
                  title: 'Open Source Project',
                  content: const Text(
                      'This project is licensed under the GPL-3.0 License, allowing free use and modification as long as the terms are followed.'),
                ),
                _infoCard(
                  icon: Symbols.devices,
                  title: 'Platform Support',
                  content: _bulletPoints(
                    [
                      'Windows x64',
                      'Linux x64',
                      'Macintosh x64',
                      'iOS 10.0+',
                      'Android 7.0+',
                    ],
                    isDarkTheme,
                  ),
                ),
                _infoCard(
                  icon: Symbols.category,
                  title: 'Modules',
                  content: _bulletPoints(
                    [
                      'Kernel - Backend, the core of the program.',
                      'Shell - Frontend, provides a command-line interface.',
                      'Script - Script control flows of the tool.',
                    ],
                    isDarkTheme,
                  ),
                ),
                _infoCard(
                  icon: Symbols.code,
                  title: 'Technology',
                  content: _detailedBulletPoints([
                    {'title': 'CMake', 'description': 'Cross-platform build system for C++.'},
                    {'title': 'Flutter', 'description': 'Beautiful UI development framework.'},
                  ], isDarkTheme),
                ),
              ],
            ),
          ),
          FooterWidget(
            onNavigate: onNavigate,
          )
        ],
      ),
    );
  }

  Widget _infoCard({required IconData icon, required String title, required Widget content}) {
    return Card(
      elevation: 4.0,
      margin: const EdgeInsets.symmetric(vertical: 12.0),
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12.0)),
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              children: [
                Icon(icon, size: 28.0, color: Colors.blueAccent),
                const SizedBox(width: 12.0),
                Text(
                  title,
                  style: const TextStyle(fontSize: 20.0, fontWeight: FontWeight.bold),
                ),
              ],
            ),
            const SizedBox(height: 12.0),
            content,
          ],
        ),
      ),
    );
  }

  Widget _bulletPoints(
    List<String> points,
    bool isDarkTheme,
  ) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: points.map((point) {
        return Padding(
          padding: const EdgeInsets.symmetric(vertical: 4.0),
          child: Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(
                '• ',
                style: TextStyle(fontSize: 16.0, color: isDarkTheme ? Colors.white54 : Colors.black54),
              ),
              Expanded(
                child: Text(
                  point,
                  style: TextStyle(fontSize: 16.0, color: isDarkTheme ? Colors.white : Colors.black87),
                ),
              ),
            ],
          ),
        );
      }).toList(),
    );
  }

  Widget _detailedBulletPoints(
    List<Map<String, String>> items,
    bool isDarKTheme,
  ) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: items.map((item) {
        return Padding(
          padding: const EdgeInsets.symmetric(vertical: 8.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(item['title']!, style: const TextStyle(fontSize: 16.0, fontWeight: FontWeight.bold)),
              const SizedBox(height: 4.0),
              Text(
                item['description']!,
                style: TextStyle(fontSize: 14.0, color: isDarKTheme ? Colors.white54 : Colors.black54),
              ),
            ],
          ),
        );
      }).toList(),
    );
  }
}
