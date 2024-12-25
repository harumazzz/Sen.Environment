import 'package:flutter/material.dart';
import 'package:hyperlink/hyperlink.dart';
import 'package:website/screen/footer/footer_widget.dart';
import 'package:website/service/download_helper.dart';

class ThankyouPage extends StatelessWidget {
  const ThankyouPage({
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
    final cardBackgroundColor = isDarkTheme ? Colors.green[900]!.withOpacity(0.5) : Colors.green[100]!.withOpacity(0.8);
    final textColor = isDarkTheme ? Colors.white : Colors.black;
    Widget buildWindowsDebug() {
      return Row(
        mainAxisSize: MainAxisSize.min,
        crossAxisAlignment: CrossAxisAlignment.center,
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Text(
            'If you having any issues with Sen on your machine, try to install ',
            textAlign: TextAlign.center,
            style: Theme.of(context).textTheme.bodyLarge?.copyWith(
                  color: textColor.withOpacity(0.9),
                ),
          ),
          HyperLink(
            text:
                '[Microsoft Visual C++ Redistributable 2015 and 2017](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170)',
            linkStyle: Theme.of(context).textTheme.bodyLarge?.copyWith(
                  color: isDarkTheme ? Colors.pink[300] : Colors.purple,
                  fontWeight: FontWeight.bold,
                ),
          ),
          const Text(' on your machine'),
        ],
      );
    }

    return Scaffold(
      appBar: AppBar(
        title: const Text('Sen: Download Page'),
      ),
      body: Center(
        child: SingleChildScrollView(
          child: Column(
            children: [
              Padding(
                padding: const EdgeInsets.all(12.0),
                child: Column(
                  children: [
                    Text(
                      'Thank you for downloading Sen: Environment!',
                      textAlign: TextAlign.center,
                      style: Theme.of(context).textTheme.headlineMedium?.copyWith(
                            color: textColor,
                            fontWeight: FontWeight.bold,
                          ),
                    ),
                    const SizedBox(height: 32.0),
                    Card(
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
                        child: Padding(
                          padding: const EdgeInsets.all(24.0),
                          child: Column(
                            mainAxisSize: MainAxisSize.min,
                            children: [
                              isWindows ? buildWindowsDebug() : const SizedBox.shrink(),
                              isWindows ? const SizedBox(height: 24.0) : const SizedBox.shrink(),
                              Text(
                                'Your download should start automatically. If not, use the link below.',
                                textAlign: TextAlign.center,
                                style: Theme.of(context).textTheme.bodyLarge?.copyWith(
                                      color: textColor.withOpacity(0.9),
                                    ),
                              ),
                              const SizedBox(height: 24.0),
                              ElevatedButton(
                                onPressed: () {
                                  DownloadHelper.downloadFile(link);
                                },
                                style: ElevatedButton.styleFrom(
                                  backgroundColor: Colors.blue,
                                ),
                                child: Padding(
                                  padding: const EdgeInsets.symmetric(
                                    vertical: 12.0,
                                    horizontal: 24.0,
                                  ),
                                  child: Text(
                                    'Download Again',
                                    style: Theme.of(context).textTheme.titleMedium?.copyWith(
                                          color: Colors.white,
                                          fontWeight: FontWeight.bold,
                                        ),
                                  ),
                                ),
                              ),
                              const SizedBox(height: 16.0),
                              TextButton(
                                onPressed: () {
                                  DownloadHelper.downloadFile(link);
                                },
                                child: Text(
                                  'Having trouble? Use the direct download link.',
                                  style: TextStyle(
                                      color: isDarkTheme ? Colors.blue[200] : Colors.blueAccent,
                                      fontWeight: FontWeight.bold),
                                ),
                              ),
                            ],
                          ),
                        ),
                      ),
                    ),
                    const SizedBox(height: 32.0),
                  ],
                ),
              ),
              FooterWidget(
                onNavigate: onNavigate,
              ),
            ],
          ),
        ),
      ),
    );
  }
}
