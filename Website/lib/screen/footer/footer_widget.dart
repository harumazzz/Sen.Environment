import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';

class FooterWidget extends StatelessWidget {
  const FooterWidget({
    super.key,
    required this.onNavigate,
  });

  final void Function(int index) onNavigate;

  Widget _supported(
    BuildContext context,
  ) {
    return Padding(
      padding: const EdgeInsets.all(24.0),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Text(
            'Sen is currently supporting for these mods and much more',
            style: Theme.of(context).textTheme.headlineSmall?.copyWith(fontWeight: FontWeight.bold),
          ),
          const SizedBox(height: 16.0),
          SingleChildScrollView(
            scrollDirection: Axis.horizontal,
            child: Row(
              children: [
                ...<String>['addendum', 'altverz', 'fallen', 'requiem', 'resonance', 'reimagined'].map(
                  (e) => Row(
                    children: [
                      Image.asset(
                        'assets/images/icons/$e.png',
                        width: 200,
                        height: 200,
                      ),
                      const SizedBox(width: 15.0),
                    ],
                  ),
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      color: Theme.of(context).brightness == Brightness.dark ? Colors.grey[900] : Colors.grey[200],
      padding: const EdgeInsets.symmetric(vertical: 24.0, horizontal: 16.0),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.center,
        children: [
          _supported(context),
          const Divider(),
          const SizedBox(height: 16.0),
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [
              Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    'Quick Links',
                    style: Theme.of(context).textTheme.titleMedium?.copyWith(fontWeight: FontWeight.bold),
                  ),
                  const SizedBox(height: 8.0),
                  TextButton(
                    onPressed: () => onNavigate(0),
                    child: const Text('Home'),
                  ),
                  TextButton(
                    onPressed: () => onNavigate(1),
                    child: const Text('Download'),
                  ),
                  TextButton(
                    onPressed: () => onNavigate(2),
                    child: const Text('Features'),
                  ),
                  TextButton(
                    onPressed: () => onNavigate(3),
                    child: const Text('Contact Us'),
                  ),
                ],
              ),
              Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    'Resources',
                    style: Theme.of(context).textTheme.titleMedium?.copyWith(fontWeight: FontWeight.bold),
                  ),
                  const SizedBox(height: 8.0),
                  TextButton(
                    onPressed: () {},
                    child: const Text('Documentation'),
                  ),
                  TextButton(
                    onPressed: () {},
                    child: const Text('Support'),
                  ),
                  TextButton(
                    onPressed: () {},
                    child: const Text('Community Forum'),
                  ),
                  TextButton(
                    onPressed: () {},
                    child: const Text('FAQ'),
                  ),
                ],
              ),
              Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    'Contact',
                    style: Theme.of(context).textTheme.titleMedium?.copyWith(fontWeight: FontWeight.bold),
                  ),
                  const SizedBox(height: 8.0),
                  const Row(
                    children: [
                      Icon(Symbols.email, size: 16.0),
                      SizedBox(width: 8.0),
                      Text('harumatsx@gmail.com'),
                    ],
                  ),
                ],
              ),
            ],
          ),
          const SizedBox(height: 16.0),
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              IconButton(
                icon: const Icon(Symbols.youtube_searched_for),
                onPressed: () {},
              ),
              IconButton(
                icon: const Icon(Icons.discord_outlined),
                onPressed: () {},
              ),
            ],
          ),
          const SizedBox(height: 16.0),
          Text(
            '© ${DateTime.now().year} copyright Haruma. All rights reserved.',
            style: Theme.of(context).textTheme.bodySmall?.copyWith(
                  color: Theme.of(context).brightness == Brightness.dark ? Colors.grey[400] : Colors.grey[600],
                ),
            textAlign: TextAlign.center,
          ),
        ],
      ),
    );
  }
}
