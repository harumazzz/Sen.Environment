import 'package:flutter/material.dart';
import 'package:hyperlink/hyperlink.dart';
import 'package:website/screen/footer/footer_widget.dart';
import 'package:website/service/download_helper.dart';

class ThankYouPage extends StatelessWidget {
  const ThankYouPage({
    super.key,
    required this.isWindows,
    required this.link,
    required this.onNavigate,
  });

  final bool isWindows;
  final String link;
  final void Function(int index) onNavigate;

  @override
  Widget build(BuildContext context) {
    final isDarkTheme = Theme.of(context).brightness == Brightness.dark;
    return Scaffold(
      appBar: AppBar(title: const Text('Sen: Download Page')),
      body: Center(
        child: SingleChildScrollView(
          child: LayoutBuilder(
            builder: (context, constraints) {
              final isSmallScreen = constraints.maxWidth < 600;

              return Padding(
                padding: EdgeInsets.all(isSmallScreen ? 12.0 : 24.0),
                child: Column(
                  spacing: 32.0,
                  children: [
                    _buildTitle(context),
                    _buildDownloadCard(context, isDarkTheme, isSmallScreen),
                    FooterWidget(onNavigate: onNavigate),
                  ],
                ),
              );
            },
          ),
        ),
      ),
    );
  }

  Widget _buildTitle(BuildContext context) {
    final textColor = Theme.of(context).brightness == Brightness.dark ? Colors.white : Colors.black;

    return Text(
      'Thank you for downloading Sen: Environment!',
      textAlign: TextAlign.center,
      style: Theme.of(context).textTheme.headlineMedium?.copyWith(
            color: textColor,
            fontWeight: FontWeight.bold,
          ),
    );
  }

  Widget _buildDownloadCard(BuildContext context, bool isDarkTheme, bool isSmallScreen) {
    final cardBackgroundColor =
        isDarkTheme ? Colors.green[900]!.withValues(alpha: 0.5) : Colors.green[100]!.withValues(alpha: 0.8);
    final textColor = isDarkTheme ? Colors.white : Colors.black;
    return Card(
      color: cardBackgroundColor,
      elevation: 0,
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(16.0),
      ),
      child: Container(
        decoration: BoxDecoration(
          border: Border.all(
            color: isDarkTheme ? Colors.lightGreen[900]! : Colors.black,
            width: 2.0,
          ),
          borderRadius: BorderRadius.circular(16.0),
        ),
        padding: EdgeInsets.all(isSmallScreen ? 16.0 : 24.0),
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            if (isWindows) ...[
              _buildWindowsDebug(context),
              const SizedBox(height: 24.0),
            ],
            Text(
              'Your download should start automatically. If not, use the link below.',
              textAlign: TextAlign.center,
              style: Theme.of(context).textTheme.bodyLarge?.copyWith(
                    color: textColor.withValues(alpha: 0.9),
                  ),
            ),
            const SizedBox(height: 24.0),
            _buildDownloadButton(context),
            const SizedBox(height: 16.0),
            _buildDirectDownloadLink(context),
          ],
        ),
      ),
    );
  }

  Widget _buildWindowsDebug(BuildContext context) {
    final isDarkTheme = Theme.of(context).brightness == Brightness.dark;
    final textColor = isDarkTheme ? Colors.white : Colors.black;
    final linkColor = isDarkTheme ? Colors.pink[300] : Colors.purple;
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 8.0),
      child: RichText(
        textAlign: TextAlign.center,
        text: TextSpan(
          style: Theme.of(context).textTheme.bodyLarge?.copyWith(
                color: textColor.withValues(alpha: 0.9),
              ),
          children: [
            const TextSpan(text: 'If you are having any issues with Sen, try installing '),
            WidgetSpan(
              alignment: PlaceholderAlignment.middle,
              child: HyperLink(
                text:
                    '[Microsoft Visual C++ Redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170)',
                linkStyle: Theme.of(context).textTheme.bodyLarge?.copyWith(
                      color: linkColor,
                      fontWeight: FontWeight.bold,
                    ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildDownloadButton(BuildContext context) {
    return ElevatedButton(
      onPressed: () {
        DownloadHelper.downloadFile(link);
      },
      style: ElevatedButton.styleFrom(backgroundColor: Colors.blue),
      child: Padding(
        padding: const EdgeInsets.symmetric(vertical: 12.0, horizontal: 24.0),
        child: Text(
          'Download Again',
          style: Theme.of(context).textTheme.titleMedium?.copyWith(
                color: Colors.white,
                fontWeight: FontWeight.bold,
              ),
        ),
      ),
    );
  }

  Widget _buildDirectDownloadLink(BuildContext context) {
    final isDarkTheme = Theme.of(context).brightness == Brightness.dark;
    final linkColor = isDarkTheme ? Colors.blue[200] : Colors.blueAccent;

    return TextButton(
      onPressed: () {
        DownloadHelper.downloadFile(link);
      },
      child: Text(
        'Having trouble? Use the direct download link.',
        style: TextStyle(color: linkColor, fontWeight: FontWeight.bold),
      ),
    );
  }
}
