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
            'Sen is currently supporting these mods and much more',
            style: Theme.of(context).textTheme.headlineSmall?.copyWith(fontWeight: FontWeight.bold),
          ),
          const SizedBox(height: 16.0),
          _buildHorizontalView(
            context: context,
          ),
        ],
      ),
    );
  }

  Widget _buildHorizontalView({
    required BuildContext context,
  }) {
    List<Widget> childList(double resolution) => [
          ...<String>['addendum', 'altverz', 'fallen', 'requiem', 'resonance', 'reimagined'].map(
            (e) => Padding(
              padding: const EdgeInsets.only(right: 15.0),
              child: Image.asset(
                'assets/images/icons/$e.png',
                width: resolution,
                height: resolution,
              ),
            ),
          ),
        ];
    if (MediaQuery.of(context).size.width < 600) {
      return SizedBox(
        height: 120,
        child: CarouselView.weighted(
          flexWeights: const <int>[1, 4, 1],
          scrollDirection: Axis.horizontal,
          children: childList(120),
        ),
      );
    }
    return SingleChildScrollView(
      scrollDirection: Axis.horizontal,
      child: Row(
        children: childList(200),
      ),
    );
  }

  Widget _buildSectionTitle(BuildContext context, String title) {
    return Text(
      title,
      style: Theme.of(context).textTheme.titleMedium?.copyWith(fontWeight: FontWeight.bold),
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
          LayoutBuilder(
            builder: (context, constraints) {
              return Wrap(
                spacing: 32.0,
                runSpacing: 16.0,
                alignment: WrapAlignment.center,
                children: [
                  _buildFooterColumn(
                    context,
                    title: 'Quick Links',
                    links: [
                      _buildNavLink('Home', 0),
                      _buildNavLink('Download', 1),
                      _buildNavLink('Features', 2),
                      _buildNavLink('Contact Us', 3),
                    ],
                  ),
                  _buildFooterColumn(
                    context,
                    title: 'Resources',
                    links: [
                      _buildNavLink('Documentation'),
                      _buildNavLink('Support'),
                      _buildNavLink('Community Forum'),
                      _buildNavLink('FAQ'),
                    ],
                  ),
                ],
              );
            },
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

  Widget _buildFooterColumn(BuildContext context, {required String title, required List<Widget> links}) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        _buildSectionTitle(context, title),
        const SizedBox(height: 8.0),
        ...links,
      ],
    );
  }

  Widget _buildNavLink(String text, [int? index]) {
    return TextButton(
      onPressed: index != null ? () => onNavigate(index) : () {},
      child: Text(text),
    );
  }
}
