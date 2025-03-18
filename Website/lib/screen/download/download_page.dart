import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';
import '../../extension/context.dart';
import '../../model/github.dart';
import '../../repository/github_repository.dart';
import '../footer/footer_widget.dart';
import '../../service/download_helper.dart';
import '../../service/service_locator.dart';
import 'download_error.dart';

class DownloadPage extends StatefulWidget {
  const DownloadPage({super.key, required this.onNavigate});
  final void Function(int index) onNavigate;

  @override
  State<DownloadPage> createState() => _DownloadPageState();

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

class _DownloadPageState extends State<DownloadPage>
    with AutomaticKeepAliveClientMixin {
  late Future<GitHub> _future;
  late int _totalDownloadCount;

  @override
  void initState() {
    _future = ServiceLocator.instance.get<GithubRepository>().getRelease();
    _totalDownloadCount = 0;
    super.initState();
  }

  void _calculateTotalDownloadCount(GitHub response) {
    if (_totalDownloadCount != 0) {
      return;
    }
    for (final e in response.assets!) {
      _totalDownloadCount += e.downloadCount!;
    }
  }

  @override
  Widget build(BuildContext context) {
    super.build(context);
    final screenWidth = MediaQuery.sizeOf(context).width;
    final isSmallScreen = screenWidth < 600;

    return FutureBuilder(
      future: _future,
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return const Center(child: CircularProgressIndicator.adaptive());
        } else if (snapshot.hasError) {
          return const DownloadError();
        } else if (snapshot.hasData) {
          final data = snapshot.data!;
          _calculateTotalDownloadCount(data);

          return SingleChildScrollView(
            child: Column(
              children: [
                Padding(
                  padding: EdgeInsets.symmetric(
                    horizontal: isSmallScreen ? 12.0 : 24.0,
                  ),
                  child: Column(
                    spacing: 16.0,
                    children: [
                      _displayLogo(isSmallScreen),
                      _introduceText(isSmallScreen),
                      _subtitleText(isSmallScreen),
                      if (isSmallScreen)
                        Column(
                          mainAxisAlignment: MainAxisAlignment.center,
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
                            Expanded(
                              child: _buildWindowsInstallation(isSmallScreen),
                            ),
                            Expanded(
                              child: _buildAndroidInstallation(isSmallScreen),
                            ),
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
        context.los.sen_title,
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
        context.los.sen_agreement,
        textAlign: TextAlign.center,
        style: Theme.of(context).textTheme.labelLarge?.copyWith(
          fontSize: isSmallScreen ? 14 : 16,
          color:
              Theme.of(context).brightness == Brightness.dark
                  ? Colors.grey[300]
                  : Colors.grey[700],
        ),
      ),
    );
  }

  Widget _downloadCount(bool isSmallScreen) {
    return Center(
      child: Text(
        context.los.total_sen_users(_totalDownloadCount),
        textAlign: TextAlign.center,
        style: Theme.of(context).textTheme.labelLarge?.copyWith(
          fontSize: isSmallScreen ? 14 : 16,
          color:
              Theme.of(context).brightness == Brightness.dark
                  ? Colors.grey[300]
                  : Colors.grey[700],
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
    const link =
        'https://github.com/Haruma-VN/Sen.Environment/releases/download/release/win-x64.zip';
    return _buildDownloadCard(
      title: 'Windows 10+',
      system: 'x64',
      fileType: 'Zip',
      imagePath: 'assets/images/logos/windows.png',
      buttonText: context.los.download_for_windows,
      link: link,
      isWindows: true,
      isSmallScreen: isSmallScreen,
    );
  }

  Widget _buildAndroidInstallation(bool isSmallScreen) {
    const link =
        'https://github.com/Haruma-VN/Sen.Environment/releases/download/release/android-arm64-v8a.zip';
    return _buildDownloadCard(
      title: 'Android 7.0+',
      system: 'arm-64',
      fileType: 'Zip',
      imagePath: 'assets/images/logos/android.png',
      buttonText: context.los.download_for_android,
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
            context.go(
              '/download/success',
              extra: <String, dynamic>{
                'isWindows': isWindows,
                'link': link,
                'onNavigate': (int index) {
                  context.go(['/', '/download', '/changelog', '/about'][index]);
                },
              },
            );
            DownloadHelper.downloadFile(link);
          },
          child: Text(
            buttonText,
            style: Theme.of(
              context,
            ).textTheme.titleMedium?.copyWith(color: Colors.white),
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
                _buildTableRow('${context.los.version}:', title),
                _buildTableRow('${context.los.system}:', system),
                _buildTableRow('${context.los.file_type}:', fileType),
              ],
            ),
          ),
        ),
      ],
    );
  }

  TableRow _buildTableRow(String label, String value) {
    return TableRow(
      children: [
        Padding(
          padding: const EdgeInsets.symmetric(vertical: 4.0),
          child: Text(label),
        ),
        Padding(
          padding: const EdgeInsets.symmetric(vertical: 4.0),
          child: Text(value),
        ),
      ],
    );
  }

  @override
  bool get wantKeepAlive => true;
}
