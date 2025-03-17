import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/material_symbols_icons.dart';
import '../../extension/context.dart';
import '../footer/footer_widget.dart';
import '../../service/url_helper.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key, required this.onNavigate});

  final void Function(int index) onNavigate;

  @override
  State<HomePage> createState() => _HomePageState();

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

class _HomePageState extends State<HomePage> {
  @override
  void initState() {
    super.initState();
  }

  Widget _displayLogo(bool isSmallScreen) {
    return Center(
      child: Image.asset(
        'assets/images/logo.png',
        width: isSmallScreen ? 100 : 150,
        height: isSmallScreen ? 100 : 150,
      ),
    );
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  @override
  void dispose() {
    super.dispose();
  }

  Widget _introduceText(BuildContext context) {
    return Center(
      child: Padding(
        padding: const EdgeInsets.symmetric(horizontal: 16.0),
        child: LayoutBuilder(
          builder: (context, constraints) {
            double fontSize = constraints.maxWidth < 400 ? 20 : 24;
            return Text(
              context.los.build_your_mod_faster,
              style: Theme.of(context).textTheme.displaySmall?.copyWith(
                fontSize: fontSize,
                fontWeight: FontWeight.bold,
              ),
              textAlign: TextAlign.center,
              softWrap: true,
            );
          },
        ),
      ),
    );
  }

  Widget _displayText(BuildContext context) {
    return Center(
      child: Padding(
        padding: const EdgeInsets.symmetric(horizontal: 16.0),
        child: LayoutBuilder(
          builder: (context, constraints) {
            double fontSize = constraints.maxWidth < 350 ? 16 : 20;
            return Text(
              context.los.sen_first_description,
              maxLines: 4,
              softWrap: true,
              textAlign: TextAlign.center,
              style: Theme.of(context).textTheme.headlineSmall?.copyWith(
                fontSize: fontSize,
                fontWeight: FontWeight.bold,
                color:
                    Theme.of(context).brightness == Brightness.dark
                        ? Colors.grey[300]
                        : Colors.grey[700],
              ),
            );
          },
        ),
      ),
    );
  }

  Widget _buttonRow() {
    return Center(
      child: Padding(
        padding: const EdgeInsets.symmetric(horizontal: 8.0),
        child: Wrap(
          spacing: 8.0,
          runSpacing: 8.0,
          alignment: WrapAlignment.center,
          children: [_downloadButton(), _viewLogButton()],
        ),
      ),
    );
  }

  Widget _downloadButton() {
    return _responsiveButton(
      text: context.los.download_now,
      color: Colors.blue,
      icon: Symbols.download,
      onPressed: () {
        widget.onNavigate(1);
      },
    );
  }

  Widget _viewLogButton() {
    return _responsiveButton(
      text: context.los.discord_server,
      icon: Icons.discord,
      color: const Color(0xFF5865F2),
      onPressed: () async {
        await UrlHelper.launch(link: 'https://discord.gg/C2Xr2kaBYJ');
      },
    );
  }

  Widget _responsiveButton({
    required String text,
    required Color color,
    required IconData icon,
    required VoidCallback onPressed,
  }) {
    return ElevatedButton(
      style: ElevatedButton.styleFrom(backgroundColor: color),
      onPressed: onPressed,
      child: Padding(
        padding: EdgeInsets.symmetric(
          vertical: 12.0,
          horizontal: MediaQuery.sizeOf(context).width < 350 ? 8.0 : 16.0,
        ),
        child: Row(
          spacing: 10.0,
          crossAxisAlignment: CrossAxisAlignment.start,
          mainAxisAlignment: MainAxisAlignment.center,
          mainAxisSize: MainAxisSize.min,
          children: [
            Icon(icon, size: 24.0, color: Colors.white),
            Text(
              text,
              style: Theme.of(context).textTheme.titleMedium?.copyWith(
                color: Colors.white,
                fontWeight: FontWeight.bold,
              ),
              textAlign: TextAlign.center,
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildDisplayPreviewImage(BuildContext context) {
    if (MediaQuery.sizeOf(context).width < 600) {
      return Image.asset(
        Theme.of(context).brightness == Brightness.dark
            ? 'assets/images/dark/phone.jpg'
            : 'assets/images/light/phone.jpg',
      );
    }
    return Image.asset(
      Theme.of(context).brightness == Brightness.dark
          ? 'assets/images/dark/launcher.png'
          : 'assets/images/light/launcher.png',
    );
  }

  Widget _previewImage(BuildContext context) {
    return Center(
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: ClipRRect(
          borderRadius: const BorderRadius.all(Radius.circular(20.0)),
          child: _buildDisplayPreviewImage(context),
        ),
      ),
    );
  }

  Widget _descriptionImage(BuildContext context) {
    if (MediaQuery.sizeOf(context).width > 600) {
      return Image.asset('assets/images/terminal.png', fit: BoxFit.cover);
    }
    return Image.asset('assets/images/phone_view.jpg', fit: BoxFit.cover);
  }

  Widget _description(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 16.0),
      child: LayoutBuilder(
        builder: (context, constraints) {
          bool isSmallScreen = constraints.maxWidth < 600;
          return Wrap(
            spacing: 16.0,
            runSpacing: 16.0,
            crossAxisAlignment: WrapCrossAlignment.center,
            alignment: WrapAlignment.center,
            children: [
              SizedBox(
                width:
                    isSmallScreen
                        ? double.infinity
                        : constraints.maxWidth * 0.6,
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  spacing: 12.0,
                  children: [
                    Text(
                      context.los.why_sen,
                      style: Theme.of(context).textTheme.headlineSmall
                          ?.copyWith(fontWeight: FontWeight.bold),
                    ),
                    _buildText(context.los.improve_mod_production),
                    _buildRichText(
                      '${context.los.community_support}: ',
                      '${context.los.sen_big_community_support}.',
                    ),
                    _buildRichText(
                      '${context.los.quality_assurance}: ',
                      '${context.los.quality_ensurance_description}.',
                    ),
                  ],
                ),
              ),
              if (!isSmallScreen) const SizedBox(width: 16.0),
              if (!isSmallScreen)
                SizedBox(
                  width:
                      isSmallScreen
                          ? constraints.maxWidth * 0.8
                          : constraints.maxWidth * 0.3,
                  child: ClipRRect(
                    borderRadius: BorderRadius.circular(20.0),
                    child: _descriptionImage(context),
                  ),
                ),
            ],
          );
        },
      ),
    );
  }

  Widget _buildText(String text) {
    return Text(
      text,
      style: Theme.of(context).textTheme.bodyLarge?.copyWith(
        color:
            Theme.of(context).brightness == Brightness.dark
                ? Colors.grey[300]
                : Colors.grey[700],
      ),
      softWrap: true,
    );
  }

  Widget _buildRichText(String boldText, String normalText) {
    return Text.rich(
      TextSpan(
        children: [
          TextSpan(
            text: boldText,
            style: Theme.of(context).textTheme.bodyLarge?.copyWith(
              fontWeight: FontWeight.bold,
              color:
                  Theme.of(context).brightness == Brightness.dark
                      ? Colors.grey[300]
                      : Colors.grey[700],
            ),
          ),
          TextSpan(
            text: normalText,
            style: Theme.of(context).textTheme.bodyLarge,
          ),
        ],
      ),
      softWrap: true,
    );
  }

  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Padding(
            padding: const EdgeInsets.all(8.0),
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              spacing: 16.0,
              children: [
                _displayLogo(MediaQuery.sizeOf(context).width < 600),
                _introduceText(context),
                _displayText(context),
                _buttonRow(),
                _previewImage(context),
                const SizedBox(height: 16.0),
                _description(context),
                const SizedBox(height: 16.0),
              ],
            ),
          ),
          FooterWidget(onNavigate: widget.onNavigate),
        ],
      ),
    );
  }
}
