import 'package:flutter/material.dart';
import 'package:website/model/github.dart';
import 'package:website/repository/github_repository.dart';
import 'package:website/screen/download/thankyou_page.dart';
import 'package:website/screen/footer/footer_widget.dart';
import 'package:website/service/download_helper.dart';
import 'package:website/service_locator/service_locator.dart';

class DownloadPage extends StatefulWidget {
  const DownloadPage({
    super.key,
    required this.onNavigate,
  });
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

  @override
  void dispose() {
    super.dispose();
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  void _calculateTotalDownloadCount(GitHub response) {
    if (_totalDownloadCount != 0) return;
    for (var e in response.assets!) {
      _totalDownloadCount += e.downloadCount!;
    }
  }

  Widget _introduceText() {
    return Center(
      child: Text(
        'Sen: Environment',
        style: Theme.of(context).textTheme.displaySmall?.copyWith(fontWeight: FontWeight.bold),
      ),
    );
  }

  Widget _agreeLicense() {
    return Center(
      child: Text(
        'By downloading and using Sen, you agree to the license terms and privacy statement.',
        style: Theme.of(context).textTheme.labelLarge?.copyWith(
              color: Theme.of(context).brightness == Brightness.dark ? Colors.grey[300] : Colors.grey[700],
            ),
      ),
    );
  }

  Widget _displayLogo() {
    return Center(
      child: Image.asset(
        'assets/images/logo.png',
        width: 150,
        height: 150,
      ),
    );
  }

  Widget _downloadCount() {
    return Center(
      child: Text(
        'There are $_totalDownloadCount users have downloaded this Sen version.',
        style: Theme.of(context).textTheme.labelLarge?.copyWith(
              color: Theme.of(context).brightness == Brightness.dark ? Colors.grey[300] : Colors.grey[700],
            ),
      ),
    );
  }

  Widget _subtitleText() {
    return Center(
      child: Text(
        'Free and open-source. Fast and efficient, low device requirement.',
        maxLines: 4,
        style: Theme.of(context).textTheme.headlineSmall?.copyWith(
              fontWeight: FontWeight.bold,
              color: Theme.of(context).brightness == Brightness.dark ? Colors.grey[300] : Colors.grey[700],
            ),
      ),
    );
  }

  Widget _buildWindowsInstallation() {
    const link = 'https://github.com/Haruma-VN/Sen.Environment/releases/download/release/win-x64.zip';
    return Column(
      crossAxisAlignment: CrossAxisAlignment.center,
      mainAxisAlignment: MainAxisAlignment.start,
      children: [
        Image.asset(
          'assets/images/logos/windows.png',
          width: 100,
          height: 100,
        ),
        const SizedBox(height: 16.0),
        ElevatedButton(
          style: ElevatedButton.styleFrom(
            backgroundColor: Colors.blue,
          ),
          onPressed: () {
            Navigator.of(context).push(
              MaterialPageRoute(
                builder: (context) => ThankyouPage(
                  link: link,
                  isWindows: true,
                  onNavigate: widget.onNavigate,
                ),
              ),
            );
            DownloadHelper.downloadFile(link);
          },
          child: Tooltip(
            message: 'Download',
            child: Padding(
              padding: const EdgeInsets.symmetric(
                vertical: 8.0,
                horizontal: 16.0,
              ),
              child: Text(
                'Download for Windows',
                style: Theme.of(context).textTheme.titleMedium?.copyWith(
                      color: Colors.white,
                    ),
              ),
            ),
          ),
        ),
        const SizedBox(height: 16.0),
        Card(
          child: Padding(
            padding: const EdgeInsets.all(16.0),
            child: Table(
              columnWidths: const {
                0: FixedColumnWidth(150),
                1: FixedColumnWidth(100),
              },
              children: const [
                TableRow(
                  children: [
                    Text('Windows version: '),
                    Text('Windows 10+'),
                  ],
                ),
                TableRow(
                  children: [
                    Text('System: '),
                    Text('x64, arm-64'),
                  ],
                ),
                TableRow(
                  children: [
                    Text('File type: '),
                    Text('Zip'),
                  ],
                ),
              ],
            ),
          ),
        ),
      ],
    );
  }

  Widget _buildAndroidInstallation() {
    const link = 'https://github.com/Haruma-VN/Sen.Environment/releases/download/release/android-arm64-v8a.zip';
    return Column(
      crossAxisAlignment: CrossAxisAlignment.center,
      mainAxisAlignment: MainAxisAlignment.start,
      children: [
        Image.asset(
          'assets/images/logos/android.png',
          width: 100,
          height: 100,
        ),
        const SizedBox(height: 16.0),
        ElevatedButton(
          style: ElevatedButton.styleFrom(
            backgroundColor: Colors.blue,
          ),
          onPressed: () {
            Navigator.of(context).push(
              MaterialPageRoute(
                builder: (context) => ThankyouPage(
                  link: link,
                  isWindows: false,
                  onNavigate: widget.onNavigate,
                ),
              ),
            );
            DownloadHelper.downloadFile(link);
          },
          child: Tooltip(
            message: 'Download',
            child: Padding(
              padding: const EdgeInsets.symmetric(
                vertical: 8.0,
                horizontal: 16.0,
              ),
              child: Text(
                'Download for Android',
                style: Theme.of(context).textTheme.titleMedium?.copyWith(
                      color: Colors.white,
                    ),
              ),
            ),
          ),
        ),
        const SizedBox(height: 16.0),
        Card(
          child: Padding(
            padding: const EdgeInsets.all(16.0),
            child: Table(
              columnWidths: const {
                0: FixedColumnWidth(150),
                1: FixedColumnWidth(100),
              },
              children: const [
                TableRow(
                  children: [
                    Text('Android version: '),
                    Text('Android 7+'),
                  ],
                ),
                TableRow(
                  children: [
                    Text('System: '),
                    Text('arm-64'),
                  ],
                ),
                TableRow(
                  children: [
                    Text('File type: '),
                    Text('Zip'),
                  ],
                ),
              ],
            ),
          ),
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return FutureBuilder(
      future: _future,
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return const Center(
            child: CircularProgressIndicator.adaptive(),
          );
        } else if (snapshot.hasError) {
          return Center(
            child: Text(snapshot.error.toString()),
          );
        } else if (snapshot.hasData) {
          final data = snapshot.data!;
          _calculateTotalDownloadCount(data);
          return SingleChildScrollView(
            scrollDirection: Axis.vertical,
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              mainAxisAlignment: MainAxisAlignment.start,
              children: [
                Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  mainAxisAlignment: MainAxisAlignment.start,
                  children: [
                    _displayLogo(),
                    const SizedBox(height: 8.0),
                    _introduceText(),
                    const SizedBox(height: 16.0),
                    _subtitleText(),
                    const SizedBox(height: 16.0),
                    Row(
                      crossAxisAlignment: CrossAxisAlignment.center,
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        _buildWindowsInstallation(),
                        const SizedBox(width: 32.0),
                        _buildAndroidInstallation(),
                      ],
                    ),
                    const SizedBox(height: 16.0),
                    _agreeLicense(),
                    const SizedBox(height: 8.0),
                    _downloadCount(),
                    const SizedBox(height: 32.0),
                  ],
                ),
                FooterWidget(
                  onNavigate: widget.onNavigate,
                )
              ],
            ),
          );
        } else {
          return const SizedBox.shrink();
        }
      },
    );
  }
}
