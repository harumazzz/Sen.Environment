import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../extension/context.dart';
import '../model/message.dart';

class ErrorPage extends StatefulWidget {
  const ErrorPage({super.key, required this.errors});
  final List<Message> errors;

  @override
  State<ErrorPage> createState() => _ErrorPageState();

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(IterableProperty<Message>('errors', errors));
  }
}

class _ErrorPageState extends State<ErrorPage> {
  late final PageController _pageController = PageController();
  int _currentPage = 0;

  void _changePage(int delta) {
    final newPage = _currentPage + delta;
    if (newPage >= 0 && newPage < widget.errors.length) {
      _pageController.animateToPage(
        newPage,
        duration: const Duration(milliseconds: 300),
        curve: Curves.easeInOut,
      );
      setState(() => _currentPage = newPage);
    }
  }

  @override
  void dispose() {
    _pageController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      mainAxisSize: MainAxisSize.min,
      children: [
        SizedBox(
          height: MediaQuery.of(context).size.height * 0.6,
          child: PageView.builder(
            controller: _pageController,
            itemCount: widget.errors.length,
            onPageChanged: (index) => setState(() => _currentPage = index),
            itemBuilder:
                (context, index) =>
                    _buildErrorContent(context, widget.errors[index]),
          ),
        ),
        _buildNavigationControls(context),
      ],
    );
  }

  Widget _buildErrorContent(BuildContext context, Message message) {
    final stack = message.subtitle?.split('\n') ?? [];
    return SingleChildScrollView(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        crossAxisAlignment: CrossAxisAlignment.start,
        children: stack.map((e) => _buildStackMessage(context, e)).toList(),
      ),
    );
  }

  Widget _buildNavigationControls(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.spaceBetween,
      children: [
        _buildNavButton(
          context.los.backward,
          Symbols.arrow_back,
          _currentPage > 0 ? () => _changePage(-1) : null,
        ),
        Row(children: List.generate(widget.errors.length, _buildIndicator)),
        _buildNavButton(
          context.los.forward,
          Symbols.arrow_forward,
          _currentPage < widget.errors.length - 1 ? () => _changePage(1) : null,
        ),
      ],
    );
  }

  Widget _buildNavButton(
    String tooltip,
    IconData icon,
    VoidCallback? onPressed,
  ) {
    return IconButton(icon: Icon(icon), onPressed: onPressed, tooltip: tooltip);
  }

  Widget _buildIndicator(int index) {
    return Container(
      margin: const EdgeInsets.symmetric(horizontal: 4.0),
      width: 8.0,
      height: 8.0,
      decoration: BoxDecoration(
        shape: BoxShape.circle,
        color: _currentPage == index ? Colors.blue : Colors.grey,
      ),
    );
  }

  Widget _buildStackMessage(BuildContext context, String message) {
    final match = RegExp(
      r'(.+?)\s+(.+?)\s+\((.+?)\)',
    ).firstMatch(message.trim());
    final functionName = match?.group(2) ?? '';
    final filePath = match?.group(3) ?? '';
    return Padding(
      padding: const EdgeInsets.symmetric(vertical: 4.0),
      child: Row(
        spacing: 8.0,
        children: [
          Icon(
            Symbols.error,
            size: 24.0,
            color: Colors.red.withValues(alpha: 0.84),
          ),
          Expanded(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  functionName,
                  style: Theme.of(context).textTheme.labelLarge?.copyWith(
                    fontWeight: FontWeight.bold,
                    color: Colors.red.withValues(alpha: 0.84),
                  ),
                ),
                if (filePath.isNotEmpty)
                  Text(
                    filePath,
                    style: Theme.of(context).textTheme.labelMedium?.copyWith(
                      fontWeight: FontWeight.bold,
                      color: Theme.of(
                        context,
                      ).colorScheme.secondary.withValues(alpha: 0.84),
                    ),
                  ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
