import 'package:flutter/material.dart';
import 'package:website/model/changelog.dart';
import 'package:website/repository/changelog_repository.dart';
import 'package:website/screen/changelog/changelog_card.dart';
import 'package:website/screen/footer/footer_widget.dart';
import 'package:website/service_locator/service_locator.dart';

class ChangelogPage extends StatefulWidget {
  const ChangelogPage({super.key, required this.onNavigate});

  final void Function(int index) onNavigate;

  @override
  State<ChangelogPage> createState() => _ChangelogPageState();
}

class _ChangelogPageState extends State<ChangelogPage> {
  late Future<List<Changelog>> _future;

  @override
  void initState() {
    _future = ServiceLocator.instance.get<ChangelogRepository>().getChangelog();
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

  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      scrollDirection: Axis.vertical,
      child: Column(
        children: [
          Padding(
            padding: const EdgeInsets.all(16.0),
            child: FutureBuilder(
              future: _future,
              builder: (context, snapshot) {
                if (snapshot.connectionState == ConnectionState.waiting) {
                  return const Center(
                    child: CircularProgressIndicator.adaptive(),
                  );
                } else if (snapshot.hasError) {
                  return Center(child: Text(snapshot.error.toString()));
                } else if (snapshot.hasData) {
                  final data = snapshot.data!;
                  return Column(
                    children: [...data.map((e) => ChangelogCard(changelog: e))],
                  );
                } else {
                  return const SizedBox.shrink();
                }
              },
            ),
          ),
          FooterWidget(onNavigate: widget.onNavigate),
        ],
      ),
    );
  }
}
