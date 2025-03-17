import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../extension/context.dart';
import '../../service/url_helper.dart';

class FooterWidget extends StatelessWidget {
  const FooterWidget({super.key, required this.onNavigate});

  final void Function(int index) onNavigate;

  Widget _supported(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(24.0),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        spacing: 16.0,
        children: [
          Text(
            context.los.sen_supported_mod,
            style: Theme.of(
              context,
            ).textTheme.headlineSmall?.copyWith(fontWeight: FontWeight.bold),
          ),
          _buildHorizontalView(context: context),
        ],
      ),
    );
  }

  Widget _buildHorizontalView({required BuildContext context}) {
    List<Widget> childList(double resolution) => [
      ...<String>[
        'addendum',
        'altverz',
        'fallen',
        'requiem',
        'resonance',
        'reimagined',
      ].map(
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
    if (MediaQuery.sizeOf(context).width < 600) {
      return SizedBox(
        height: 120,
        child: CarouselView.weighted(
          flexWeights: const <int>[1, 4, 1],
          children: childList(120),
        ),
      );
    }
    return SingleChildScrollView(
      scrollDirection: Axis.horizontal,
      child: Row(children: childList(200)),
    );
  }

  Widget _buildSectionTitle(BuildContext context, String title) {
    return Text(
      title,
      style: Theme.of(
        context,
      ).textTheme.titleMedium?.copyWith(fontWeight: FontWeight.bold),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      color:
          Theme.of(context).brightness == Brightness.dark
              ? Colors.grey[900]
              : Colors.grey[200],
      padding: const EdgeInsets.symmetric(vertical: 24.0, horizontal: 16.0),
      child: Column(
        spacing: 16.0,
        children: [
          _supported(context),
          const Divider(),
          LayoutBuilder(
            builder: (context, constraints) {
              return Wrap(
                spacing: 32.0,
                runSpacing: 16.0,
                alignment: WrapAlignment.center,
                children: [
                  _buildFooterColumn(
                    context,
                    title: context.los.quick_links,
                    links: [
                      _buildNavLink(context.los.home, 0),
                      _buildNavLink(context.los.download, 1),
                      _buildNavLink(context.los.changelog, 2),
                      _buildNavLink(context.los.contact, 3),
                    ],
                  ),
                  _buildFooterColumn(
                    context,
                    title: context.los.resources,
                    links: [
                      _buildNavigatorLink(
                        context.los.documentation,
                        'https://github.com/harumazzz/Sen.Environment',
                      ),
                      _buildNavigatorLink(
                        context.los.support,
                        'https://discord.gg/C2Xr2kaBYJ',
                      ),
                      _buildNavigatorLink(
                        context.los.community_forum,
                        'https://discord.com/invite/pvz',
                      ),
                      _buildNavigatorLink(
                        context.los.faq,
                        'https://github.com/harumazzz/Sen.Environment/issues',
                      ),
                    ],
                  ),
                ],
              );
            },
          ),
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              IconButton(
                icon: const Icon(Symbols.youtube_activity),
                onPressed: () async {
                  await UrlHelper.launch(
                    link: 'https://www.youtube.com/@harumavn',
                  );
                },
              ),
              IconButton(
                icon: const Icon(Icons.discord_outlined),
                onPressed: () async {
                  await UrlHelper.launch(link: 'https://discord.gg/C2Xr2kaBYJ');
                },
              ),
            ],
          ),
          Text(
            '© ${DateTime.now().year} copyright Haruma. All rights reserved.',
            style: Theme.of(context).textTheme.bodySmall?.copyWith(
              color:
                  Theme.of(context).brightness == Brightness.dark
                      ? Colors.grey[400]
                      : Colors.grey[600],
            ),
            textAlign: TextAlign.center,
          ),
        ],
      ),
    );
  }

  Widget _buildFooterColumn(
    BuildContext context, {
    required String title,
    required List<Widget> links,
  }) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      spacing: 8.0,
      children: [_buildSectionTitle(context, title), ...links],
    );
  }

  Widget _buildNavLink(String text, [int? index]) {
    return TextButton(
      onPressed: index != null ? () => onNavigate(index) : () {},
      child: Text(text),
    );
  }

  Widget _buildNavigatorLink(String text, [String? link]) {
    return TextButton(
      onPressed:
          link != null ? () async => await UrlHelper.launch(link: link) : () {},
      child: Text(text),
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
  }
}
