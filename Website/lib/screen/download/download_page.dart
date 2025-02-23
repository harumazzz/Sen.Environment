import 'package:flutter/material.dart';
import 'package:website/model/github.dart';
import 'package:website/repository/github_repository.dart';
import 'package:website/screen/download/thankyou_page.dart';
import 'package:website/screen/footer/footer_widget.dart';
import 'package:website/service/download_helper.dart';
import 'package:website/service_locator/service_locator.dart';

class DownloadPage extends StatefulWidget {
  const DownloadPage({super.key, required this.onNavigate});
  final void Function(int index) onNavigate;

  @override
  State<DownloadPage> createState() => _DownloadPageState();
}

class _DownloadPageState extends State<DownloadPage> {
  late Future<GitHub> _future;
  late int _totalDownloadCount;

  @override
  void initState() {
    _future = ServiceLocator.instance.get<GithubRepository>().getRelease();
    _totalDownloadCount = 0;
    super.initState();
  }

  void _calculateTotalDownloadCount(GitHub response) {
    if (_totalDownloadCount != 0) return;
    for (var e in response.assets!) {
      _totalDownloadCount += e.downloadCount!;
    }
  }

  @override
  Widget build(BuildContext context) {
    final screenWidth = MediaQuery.of(context).size.width;
    final isSmallScreen = screenWidth < 600;

    return FutureBuilder(
      future: _future,
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return const Center(child: CircularProgressIndicator.adaptive());
        } else if (snapshot.hasError) {
          return Center(child: Text(snapshot.error.toString()));
        } else if (snapshot.hasData) {
          final data = snapshot.data!;
          _calculateTotalDownloadCount(data);

          return SingleChildScrollView(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.center,
              children: [
                Padding(
                  padding: EdgeInsets.symmetric(horizontal: isSmallScreen ? 12.0 : 24.0),
                  child: Column(
                    spacing: 16.0,
                    children: [
                      _displayLogo(isSmallScreen),
                      _introduceText(isSmallScreen),
                      _subtitleText(isSmallScreen),
                      if (isSmallScreen)
                        Column(
                          mainAxisAlignment: MainAxisAlignment.center,
                          crossAxisAlignment: CrossAxisAlignment.center,
                          spacing: 24.0,
                          children: [
                            _buildWindowsInstallation(isSmallScreen),
                            _buildAndroidInstallation(isSmallScreen),
                          ],
                        )
                      else
                        Row(
                          mainAxisAlignment: MainAxisAlignment.center,
                          spacing: 32.0,
                          children: [
                            Expanded(child: _buildWindowsInstallation(isSmallScreen)),
                            Expanded(child: _buildAndroidInstallation(isSmallScreen)),
                          ],
                        ),
                      _agreeLicense(isSmallScreen),
                      _downloadCount(isSmallScreen),
                      const SizedBox(height: 16.0),
                    ],
                  ),
                ),
                FooterWidget(onNavigate: widget.onNavigate),
              ],
            ),
          );
        } else {
          return const SizedBox.shrink();
        }
      },
    );
  }

  Widget _introduceText(bool isSmallScreen) {
    return Center(
      child: Text(
        'Sen: Environment',
        textAlign: TextAlign.center,
        style: Theme.of(context).textTheme.displaySmall?.copyWith(
              fontSize: isSmallScreen ? 20 : 28,
              fontWeight: FontWeight.bold,
            ),
      ),
    );
  }

  Widget _subtitleText(bool isSmallScreen) {
    return Center(
      child: Text(
        'Free and open-source. Fast and efficient, low device requirement.',
        maxLines: 4,
        textAlign: TextAlign.center,
        style: Theme.of(context).textTheme.headlineSmall?.copyWith(
              fontSize: isSmallScreen ? 16 : 20,
              fontWeight: FontWeight.bold,
            ),
      ),
    );
  }

  Widget _agreeLicense(bool isSmallScreen) {
    return Center(
      child: Text(
        'By downloading and using Sen, you agree to the license terms and privacy statement.',
        textAlign: TextAlign.center,
        style: Theme.of(context).textTheme.labelLarge?.copyWith(
              fontSize: isSmallScreen ? 14 : 16,
              color: Theme.of(context).brightness == Brightness.dark ? Colors.grey[300] : Colors.grey[700],
            ),
      ),
    );
  }

  Widget _downloadCount(bool isSmallScreen) {
    return Center(
      child: Text(
        'There are $_totalDownloadCount users who have downloaded this Sen version.',
        textAlign: TextAlign.center,
        style: Theme.of(context).textTheme.labelLarge?.copyWith(
              fontSize: isSmallScreen ? 14 : 16,
              color: Theme.of(context).brightness == Brightness.dark ? Colors.grey[300] : Colors.grey[700],
            ),
      ),
    );
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

  Widget _buildWindowsInstallation(bool isSmallScreen) {
    const link = 'https://github.com/Haruma-VN/Sen.Environment/releases/download/release/win-x64.zip';
    return _buildDownloadCard(
      title: 'Windows 10+',
      system: 'x64',
      fileType: 'Zip',
      imagePath: 'assets/images/logos/windows.png',
      buttonText: 'Download for Windows',
      link: link,
      isWindows: true,
      isSmallScreen: isSmallScreen,
    );
  }

  Widget _buildAndroidInstallation(bool isSmallScreen) {
    const link = 'https://github.com/Haruma-VN/Sen.Environment/releases/download/release/android-arm64-v8a.zip';
    return _buildDownloadCard(
      title: 'Android 7.0+',
      system: 'arm-64',
      fileType: 'Zip',
      imagePath: 'assets/images/logos/android.png',
      buttonText: 'Download for Android',
      link: link,
      isWindows: false,
      isSmallScreen: isSmallScreen,
    );
  }

  Widget _buildDownloadCard({
    required String title,
    required String system,
    required String fileType,
    required String imagePath,
    required String buttonText,
    required String link,
    required bool isWindows,
    required bool isSmallScreen,
  }) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.center,
      spacing: 16.0,
      children: [
        Image.asset(
          imagePath,
          width: isSmallScreen ? 80 : 100,
          height: isSmallScreen ? 80 : 100,
        ),
        ElevatedButton(
          style: ElevatedButton.styleFrom(
            backgroundColor: Colors.blue,
            padding: EdgeInsets.symmetric(
              vertical: isSmallScreen ? 12.0 : 20.0,
              horizontal: isSmallScreen ? 16.0 : 24.0,
            ),
          ),
          onPressed: () {
            Navigator.of(context).push(
              MaterialPageRoute(
                builder: (context) => ThankYouPage(
                  link: link,
                  isWindows: isWindows,
                  onNavigate: widget.onNavigate,
                ),
              ),
            );
            DownloadHelper.downloadFile(link);
          },
          child: Text(
            buttonText,
            style: Theme.of(context).textTheme.titleMedium?.copyWith(color: Colors.white),
          ),
        ),
        Card(
          child: Padding(
            padding: const EdgeInsets.all(12.0),
            child: Table(
              columnWidths: const {
                0: FixedColumnWidth(150),
                1: FixedColumnWidth(100),
              },
              children: [
                _buildTableRow('Version:', title),
                _buildTableRow('System:', system),
                _buildTableRow('File type:', fileType),
              ],
            ),
          ),
        ),
      ],
    );
  }

  TableRow _buildTableRow(
    String label,
    String value,
  ) {
    return TableRow(children: [
      Padding(padding: const EdgeInsets.symmetric(vertical: 4.0), child: Text(label)),
      Padding(padding: const EdgeInsets.symmetric(vertical: 4.0), child: Text(value)),
    ]);
  }
}
