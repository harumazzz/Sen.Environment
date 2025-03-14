import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../extension/context.dart';
import '../../model/translator.dart';
import '../../widget/hyperlink.dart';

class TranslatorPage extends StatelessWidget {
  const TranslatorPage({super.key, required this.translator});

  final Translator translator;

  bool _isLink(String value) => RegExp(
    r'((([A-Za-z]{3,9}:(?:\/\/)?)(?:[-;:&=\+\$,\w]+@)?[A-Za-z0-9.-]+|(?:www.|[-;:&=\+\$,\w]+@)[A-Za-z0-9.-]+)((?:\/[\+~%\/.\w-_]*)?\??(?:[-\+=&;%@.\w_]*)#?(?:[\w]*))?)',
  ).hasMatch(value);

  @override
  Widget build(BuildContext context) {
    final colorScheme = Theme.of(context).colorScheme;
    final textTheme = Theme.of(context).textTheme;
    return Stack(
      children: [
        Card(
          elevation: 3,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(16),
          ),
          child: SingleChildScrollView(
            child: Padding(
              padding: const EdgeInsets.symmetric(
                vertical: 16.0,
                horizontal: 24.0,
              ),
              child: Column(
                mainAxisSize: MainAxisSize.min,
                children: [
                  Container(
                    decoration: BoxDecoration(
                      shape: BoxShape.circle,
                      border: Border.all(color: colorScheme.primary, width: 2),
                    ),
                    child: CircleAvatar(
                      radius: 50,
                      backgroundImage: AssetImage(translator.imageCover),
                    ),
                  ),
                  const SizedBox(height: 16),
                  Text(
                    translator.name,
                    style: textTheme.titleLarge?.copyWith(
                      fontWeight: FontWeight.bold,
                      color: colorScheme.onSurface,
                    ),
                    textAlign: TextAlign.center,
                  ),
                  const SizedBox(height: 4),
                  Text(
                    context.los.translator,
                    style: textTheme.bodyMedium?.copyWith(
                      color: colorScheme.primary,
                    ),
                  ),
                  const SizedBox(height: 16),
                  Divider(
                    thickness: 1,
                    height: 24,
                    color: colorScheme.outlineVariant,
                  ),
                  if (translator.discord != null) ...[
                    ListTile(
                      leading: const Icon(
                        Icons.discord_outlined,
                        color: Colors.white,
                        size: 24,
                      ),
                      title: SelectableText(
                        translator.discord!,
                        style: textTheme.bodyMedium?.copyWith(
                          color: colorScheme.secondary,
                        ),
                      ),
                    ),
                    Divider(
                      thickness: 1,
                      height: 24,
                      color: colorScheme.outlineVariant,
                    ),
                  ],
                  Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      ...translator.contacts.map((e) {
                        final isLink = _isLink(e[1]);
                        return Padding(
                          padding: const EdgeInsets.symmetric(vertical: 6),
                          child: ListTile(
                            leading:
                                isLink
                                    ? const Icon(
                                      Symbols.link,
                                      color: Colors.grey,
                                    )
                                    : const Icon(
                                      Symbols.person,
                                      color: Colors.grey,
                                    ),
                            title: Text(e[0], style: textTheme.bodyMedium),
                            subtitle:
                                isLink
                                    ? Hyperlink(title: e[0], link: e[1])
                                    : SelectableText(
                                      e[1],
                                      style: textTheme.bodyMedium,
                                    ),
                          ),
                        );
                      }),
                    ],
                  ),
                ],
              ),
            ),
          ),
        ),
        Positioned(
          top: 16,
          right: 16,
          child: IconButton(
            icon: const Icon(Symbols.close),
            onPressed: () {
              Navigator.of(context).pop();
            },
          ),
        ),
      ],
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Translator>('translator', translator));
  }
}
