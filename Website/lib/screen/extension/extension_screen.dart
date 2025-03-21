import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:shimmer/shimmer.dart';

import '../../extension/context.dart';
import '../../model/extension.dart';
import '../../repository/extension_repository.dart';
import '../../service/service_locator.dart';
import '../../service/url_helper.dart';
import '../footer/footer_widget.dart';

class ExtensionsPage extends StatefulWidget {
  const ExtensionsPage({super.key, required this.onNavigate});

  final void Function(int index) onNavigate;

  @override
  State<ExtensionsPage> createState() => _ExtensionsPageState();

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

class _ExtensionsPageState extends State<ExtensionsPage> {
  late ScrollController _scrollController;
  late TextEditingController _searchController;
  late bool _isFetching;
  late bool _hasMore;
  late List<Extension> _extensions;

  @override
  void initState() {
    super.initState();
    _extensions = [];
    _isFetching = false;
    _hasMore = true;
    _searchController = TextEditingController();
    _scrollController = ScrollController()..addListener(_onScroll);
  }

  @override
  void dispose() {
    _searchController.dispose();
    _scrollController.dispose();
    super.dispose();
  }

  Future<void> _fetchExtensions() async {
    if (_isFetching || !_hasMore) {
      return;
    }

    setState(() => _isFetching = true);

    final newExtensions =
        await ServiceLocator.instance
            .get<ExtensionRepository>()
            .getExtensions();

    setState(() {
      _extensions.addAll(newExtensions);
      _hasMore = ServiceLocator.instance.get<ExtensionRepository>().canFetch;
      _isFetching = false;
    });
  }

  void _onScroll() {
    if (_scrollController.position.pixels >=
            _scrollController.position.maxScrollExtent - 200 &&
        !_isFetching &&
        _hasMore) {
      _fetchExtensions();
    }
  }

  Future<void> _launchDownload(Extension e) async {
    void showSuccess() {
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(
          content: Text(
            context.los.download_success,
            softWrap: true,
            overflow: TextOverflow.visible,
          ),
          width: MediaQuery.sizeOf(context).width > 800 ? 400 : null,
          behavior: SnackBarBehavior.floating,
          duration: const Duration(milliseconds: 1500),
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(10.0),
          ),
        ),
      );
    }

    await UrlHelper.launch(link: e.downloadLink);
    showSuccess();
  }

  Widget _buildShimmer() {
    return Shimmer.fromColors(
      baseColor: Colors.grey[300]!,
      highlightColor: Colors.grey[100]!,
      child: Column(
        children: [
          _buildShimmerItem(),
          _buildShimmerItem(),
          _buildShimmerItem(),
          _buildShimmerItem(),
          _buildShimmerItem(),
        ],
      ),
    );
  }

  Widget _buildShimmerItem() {
    return Padding(
      padding: const EdgeInsets.symmetric(vertical: 8.0),
      child: Card(
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(10.0),
        ),
        elevation: 3,
        child: ListTile(
          leading: _buildShimmerBox(width: 40, height: 40),
          title: _buildShimmerBox(height: 16),
          subtitle: _buildShimmerBox(height: 14),
          trailing: _buildShimmerBox(width: 24, height: 24),
        ),
      ),
    );
  }

  Widget _buildShimmerBox({
    double width = double.infinity,
    required double height,
  }) {
    return Container(width: width, height: height, color: Colors.white);
  }

  Widget _buildExtensionItem(Extension ext) {
    return Card(
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10.0)),
      elevation: 3,
      child: ListTile(
        leading: const Icon(Symbols.package_2, color: Colors.blueAccent),
        title: Text(
          ext.extensionTitle,
          style: const TextStyle(fontWeight: FontWeight.bold),
        ),
        subtitle: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          spacing: 4.0,
          children: [
            Text(
              ext.extensionDescription,
              maxLines: 8,
              overflow: TextOverflow.ellipsis,
            ),
            Text(
              '${context.los.author}: ${ext.extensionAuthor}',
              style: TextStyle(
                color: Colors.grey[600],
                fontStyle: FontStyle.italic,
              ),
            ),
          ],
        ),
        trailing: IconButton(
          tooltip: context.los.download,
          icon: const Icon(Symbols.download, color: Colors.green),
          onPressed: () => _launchDownload(ext),
        ),
      ),
    );
  }

  Widget _buildSearch() {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: Row(
        children: [
          Expanded(
            child: TextFormField(
              controller: _searchController,
              decoration: InputDecoration(
                prefixIcon: const Icon(Symbols.search),
                border: OutlineInputBorder(
                  borderRadius: BorderRadius.circular(10),
                ),
                labelText: context.los.modules,
                suffix: IconButton(
                  onPressed: () {
                    _searchController.clear();
                  },
                  icon: const Icon(Symbols.clear),
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      controller: _scrollController,
      child: Column(
        children: [
          _buildSearch(),
          Padding(
            padding: const EdgeInsets.all(8.0),
            child: FutureBuilder(
              future: _fetchExtensions(),
              builder: (context, snapshot) {
                if (snapshot.connectionState == ConnectionState.waiting &&
                    _extensions.isEmpty) {
                  return _buildShimmer();
                }
                return Column(
                  children: [
                    ..._extensions.map((ext) => _buildExtensionItem(ext)),
                    if (_isFetching)
                      Padding(
                        padding: const EdgeInsets.all(8.0),
                        child: _buildShimmer(),
                      ),
                  ],
                );
              },
            ),
          ),
          FooterWidget(onNavigate: widget.onNavigate),
        ],
      ),
    );
  }
}
