import 'package:flutter/material.dart';
import 'package:sen/model/translator.dart';
import 'package:sen/widget/hyperlink.dart';

class TranslatorPage extends StatelessWidget {
  const TranslatorPage({
    super.key,
    required this.translator,
  });

  final Translator translator;

  bool _isLink(String value) => RegExp(
          r'((([A-Za-z]{3,9}:(?:\/\/)?)(?:[-;:&=\+\$,\w]+@)?[A-Za-z0-9.-]+|(?:www.|[-;:&=\+\$,\w]+@)[A-Za-z0-9.-]+)((?:\/[\+~%\/.\w-_]*)?\??(?:[-\+=&;%@.\w_]*)#?(?:[\w]*))?)')
      .hasMatch(value);

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.center,
      mainAxisSize: MainAxisSize.min,
      children: [
        CircleAvatar(
          radius: 40.0,
          backgroundImage: AssetImage(translator.imageCover),
        ),
        const SizedBox(height: 16.0),
        Text(
          translator.name,
          style: Theme.of(context).textTheme.titleLarge?.copyWith(
                fontWeight: FontWeight.bold,
                color: Theme.of(context).colorScheme.onSurface,
              ),
          textAlign: TextAlign.center,
        ),
        const SizedBox(height: 8.0),
        Divider(
          color: Colors.grey.withValues(alpha: 0.3),
          thickness: 1.0,
          height: 24.0,
        ),
        const SizedBox(height: 8.0),
        if (translator.discord != null)
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              const Icon(
                Icons.discord_outlined,
                color: Colors.grey,
                size: 20.0,
              ),
              const SizedBox(width: 8.0),
              Row(
                children: [
                  Text(
                    "Discord: ",
                    style: Theme.of(context).textTheme.bodyMedium?.copyWith(
                          color: Theme.of(context).colorScheme.secondary,
                        ),
                  ),
                  SelectableText(
                    translator.discord!,
                    style: Theme.of(context).textTheme.bodyMedium?.copyWith(
                          color: Theme.of(context).colorScheme.secondary,
                        ),
                  ),
                ],
              ),
            ],
          ),
        if (translator.discord != null) const SizedBox(height: 16.0),
        Divider(
          color: Colors.grey.withValues(alpha: 0.3),
          thickness: 1.0,
          height: 24.0,
        ),
        Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: translator.contacts.map((e) {
            final isLink = _isLink(e[1]);
            return Padding(
              padding: const EdgeInsets.symmetric(vertical: 6.0),
              child: Row(
                children: [
                  isLink
                      ? const Icon(
                          Icons.link_outlined,
                          color: Colors.grey,
                          size: 20.0,
                        )
                      : Text(e[0]),
                  const SizedBox(width: 8.0),
                  isLink ? Hyperlink(title: e[0], link: e[1]) : Text(e[1]),
                ],
              ),
            );
          }).toList(),
        ),
      ],
    );
  }
}
