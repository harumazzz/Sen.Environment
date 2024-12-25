import 'package:flutter/material.dart';
import 'package:website/screen/footer/footer_widget.dart';
import 'package:website/service/url_helper.dart';

class HomePage extends StatefulWidget {
  const HomePage({
    super.key,
    required this.onNavigate,
  });

  final void Function(int index) onNavigate;

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  @override
  void initState() {
    super.initState();
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
  }

  @override
  void dispose() {
    super.dispose();
  }

  Widget _introduceText() {
    return Center(
      child: Text(
        'Build your PvZ2 mod faster!',
        style: Theme.of(context).textTheme.displaySmall?.copyWith(fontWeight: FontWeight.bold),
      ),
    );
  }

  Widget _displayText() {
    return Center(
      child: Text(
        'Sen is what you need! The all-in-one tool, is recommended by most modders.',
        maxLines: 4,
        style: Theme.of(context).textTheme.headlineSmall?.copyWith(
              fontWeight: FontWeight.bold,
              color: Theme.of(context).brightness == Brightness.dark ? Colors.grey[300] : Colors.grey[700],
            ),
      ),
    );
  }

  Widget _downloadButton() {
    return ElevatedButton(
      style: ElevatedButton.styleFrom(
        backgroundColor: Colors.blue,
      ),
      onPressed: () async {
        widget.onNavigate(1);
      },
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Text(
          'Download now',
          style: Theme.of(context).textTheme.titleMedium?.copyWith(
                color: Colors.white,
                fontWeight: FontWeight.bold,
              ),
        ),
      ),
    );
  }

  Widget _viewLogButton() {
    return ElevatedButton(
      style: ElevatedButton.styleFrom(
        backgroundColor: const Color(0xFF5865F2),
      ),
      onPressed: () async {
        await UrlHelper.launch(link: 'https://discord.gg/C2Xr2kaBYJ');
      },
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Text(
          'Join discord community',
          style: Theme.of(context).textTheme.titleMedium?.copyWith(
                color: Colors.white,
                fontWeight: FontWeight.bold,
              ),
        ),
      ),
    );
  }

  Widget _previewImage() {
    return Center(
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: ClipRRect(
          borderRadius: const BorderRadius.all(Radius.circular(20.0)),
          child: Image.asset(Theme.of(context).brightness == Brightness.dark
              ? 'assets/images/dark/launcher.png'
              : 'assets/images/light/launcher.png'),
        ),
      ),
    );
  }

  Widget _description() {
    return Row(
      crossAxisAlignment: CrossAxisAlignment.start,
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Flexible(
          flex: 3,
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            mainAxisAlignment: MainAxisAlignment.start,
            children: [
              Text(
                'Why Sen?',
                style: Theme.of(context).textTheme.headlineSmall?.copyWith(fontWeight: FontWeight.bold),
              ),
              const SizedBox(height: 16.0),
              Text(
                'Improve your mod production speed by 20% with Sen',
                style: Theme.of(context).textTheme.bodyLarge?.copyWith(
                      color: Theme.of(context).brightness == Brightness.dark ? Colors.grey[300] : Colors.grey[700],
                    ),
                softWrap: true,
              ),
              const SizedBox(height: 16.0),
              Text.rich(
                TextSpan(
                  children: [
                    TextSpan(
                      text: 'Community Support: ',
                      style: Theme.of(context).textTheme.bodyLarge?.copyWith(
                            fontWeight: FontWeight.bold,
                            color:
                                Theme.of(context).brightness == Brightness.dark ? Colors.grey[300] : Colors.grey[700],
                          ),
                    ),
                    TextSpan(
                      text: 'Sen comes with a big community to help you modify the game easily.',
                      style: Theme.of(context).textTheme.bodyLarge,
                    ),
                  ],
                ),
                softWrap: true,
              ),
              const SizedBox(height: 16.0),
              Text.rich(
                TextSpan(
                  children: [
                    TextSpan(
                      text: 'Quality Assurance: ',
                      style: Theme.of(context).textTheme.bodyLarge?.copyWith(
                            fontWeight: FontWeight.bold,
                            color:
                                Theme.of(context).brightness == Brightness.dark ? Colors.grey[300] : Colors.grey[700],
                          ),
                    ),
                    TextSpan(
                      text: 'Sen offers tons of tools to streamline your workflow.',
                      style: Theme.of(context).textTheme.bodyLarge,
                    ),
                  ],
                ),
                softWrap: true,
              ),
            ],
          ),
        ),
        const SizedBox(width: 32.0),
        Flexible(
          flex: 2,
          child: ClipRRect(
            borderRadius: const BorderRadius.all(Radius.circular(20.0)),
            child: Image.asset(
              'assets/images/terminal.png',
            ),
          ),
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        mainAxisAlignment: MainAxisAlignment.start,
        children: [
          Padding(
            padding: const EdgeInsets.all(8.0),
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              mainAxisAlignment: MainAxisAlignment.start,
              children: [
                _introduceText(),
                const SizedBox(height: 16.0),
                _displayText(),
                const SizedBox(height: 16.0),
                Row(
                  crossAxisAlignment: CrossAxisAlignment.center,
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    _downloadButton(),
                    const SizedBox(width: 8.0),
                    _viewLogButton(),
                  ],
                ),
                const SizedBox(height: 16.0),
                _previewImage(),
                const SizedBox(height: 32.0),
                _description(),
                const SizedBox(height: 32.0),
              ],
            ),
          ),
          FooterWidget(
            onNavigate: widget.onNavigate,
          ),
        ],
      ),
    );
  }
}
