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
  late PageController _pageController;

  late int _currentPage;

  @override
  void initState() {
    _currentPage = 0;
    _pageController = PageController();
    super.initState();
  }

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
    if (widget.errors.isEmpty) {
      return const CustomNoErrorPage();
    }
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
            itemBuilder: (context, index) {
              return CustomErrorContent(message: widget.errors[index]);
            },
          ),
        ),
        CustomNavigationControl(
          onBackward: _currentPage > 0 ? () => _changePage(-1) : null,
          onForward:
              _currentPage < widget.errors.length - 1
                  ? () => _changePage(1)
                  : null,
          size: widget.errors.length,
          colorOf: (index) {
            Color? exchangeColor() {
              return _currentPage == index
                  ? Colors.blue.withValues(alpha: 0.84)
                  : Colors.grey.withValues(alpha: 0.84);
            }

            return exchangeColor();
          },
        ),
      ],
    );
  }
}

class CustomErrorContent extends StatelessWidget {
  const CustomErrorContent({super.key, required this.message});

  final Message message;

  @override
  Widget build(BuildContext context) {
    final stack = message.subtitle?.split('\n') ?? [];
    return SingleChildScrollView(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [...stack.map((e) => CustomStackMessage(message: e))],
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Message>('message', message));
  }
}

class CustomStackMessage extends StatelessWidget {
  const CustomStackMessage({super.key, required this.message});

  final String message;

  @override
  Widget build(BuildContext context) {
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

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('message', message));
  }
}

class CustomIndicator extends StatelessWidget {
  const CustomIndicator({super.key, required this.index, required this.color});

  final int index;

  final Color? color;

  @override
  Widget build(BuildContext context) {
    return Container(
      margin: const EdgeInsets.symmetric(horizontal: 4.0),
      width: 8.0,
      height: 8.0,
      decoration: BoxDecoration(shape: BoxShape.circle, color: color),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(ColorProperty('color', color));
    properties.add(IntProperty('index', index));
  }
}

class CustomNavigationButton extends StatelessWidget {
  const CustomNavigationButton({
    super.key,
    required this.icon,
    required this.tooltip,
    this.onPressed,
  });

  final IconData icon;
  final String tooltip;
  final void Function()? onPressed;

  @override
  Widget build(BuildContext context) {
    return IconButton(icon: Icon(icon), onPressed: onPressed, tooltip: tooltip);
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<IconData>('icon', icon));
    properties.add(StringProperty('tooltip', tooltip));
    properties.add(
      ObjectFlagProperty<void Function()?>.has('onPressed', onPressed),
    );
  }
}

class CustomNavigationControl extends StatelessWidget {
  const CustomNavigationControl({
    super.key,
    required this.onBackward,
    required this.onForward,
    required this.size,
    required this.colorOf,
  });

  final void Function()? onBackward;

  final void Function()? onForward;

  final int size;

  final Color? Function(int index) colorOf;

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.spaceBetween,
      children: [
        CustomNavigationButton(
          tooltip: context.los.backward,
          icon: Symbols.arrow_back,
          onPressed: onBackward,
        ),
        Row(
          children: List<Widget>.generate(size, (index) {
            return CustomIndicator(index: index, color: colorOf(index));
          }),
        ),
        CustomNavigationButton(
          tooltip: context.los.forward,
          icon: Symbols.arrow_forward,
          onPressed: onForward,
        ),
      ],
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<void Function()>.has('onForward', onForward),
    );
    properties.add(
      ObjectFlagProperty<void Function()>.has('onBackward', onBackward),
    );
    properties.add(
      ObjectFlagProperty<Color? Function(int index)>.has('colorOf', colorOf),
    );
    properties.add(IntProperty('size', size));
  }
}

class CustomNoErrorPage extends StatelessWidget {
  const CustomNoErrorPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Text(
      '${context.los.no_error_found}!',
      style: Theme.of(context).textTheme.labelLarge,
    );
  }
}
