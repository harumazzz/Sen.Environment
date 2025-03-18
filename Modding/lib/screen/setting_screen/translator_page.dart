import 'package:flutter/material.dart';
import 'package:flutter/foundation.dart';
import 'package:material_symbols_icons/symbols.dart';

import '../../extension/context.dart';
import '../../model/translator.dart';
import '../../widget/hyperlink.dart';

class TranslatorPage extends StatelessWidget {
  const TranslatorPage({super.key, required this.translator});

  final Translator translator;

  bool _isLink(String value) {
    final urlRegex = RegExp(
      r'((([A-Za-z]{3,9}:(?:\/\/)?)(?:[-;:&=+\$,\w]+@)?[A-Za-z0-9.-]+|(?:www.|[-;:&=+\$,\w]+@)[A-Za-z0-9.-]+)((?:\/[\+~%\/.\w-_]*)?\??(?:[-\+=&;%@.\w_]*)#?(?:[\w]*))?)',
    );
    return urlRegex.hasMatch(value);
  }

  @override
  Widget build(BuildContext context) {
    final colorScheme = Theme.of(context).colorScheme;
    final textTheme = Theme.of(context).textTheme;

    return Stack(
      children: [
        _buildCard(context, colorScheme, textTheme),
        _buildCloseButton(context),
      ],
    );
  }

  Widget _buildCard(
    BuildContext context,
    ColorScheme colorScheme,
    TextTheme textTheme,
  ) {
    return Card(
      elevation: 3,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16.0)),
      child: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.symmetric(vertical: 16.0, horizontal: 24.0),
          child: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              _buildProfileSection(context, colorScheme, textTheme),
              const SizedBox(height: 16.0),
              const Divider(thickness: 1.0),
              if (translator.discord != null)
                _buildDiscordTile(textTheme, colorScheme),
              _buildContactsList(textTheme),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildProfileSection(
    BuildContext context,
    ColorScheme colorScheme,
    TextTheme textTheme,
  ) {
    return Column(
      children: [
        CircleAvatar(
          radius: 50,
          backgroundImage: AssetImage(translator.imageCover),
          backgroundColor: colorScheme.surface,
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
          style: textTheme.bodyMedium?.copyWith(color: colorScheme.primary),
        ),
      ],
    );
  }

  Widget _buildDiscordTile(TextTheme textTheme, ColorScheme colorScheme) {
    return Column(
      children: [
        ListTile(
          leading: const Icon(
            Icons.discord_outlined,
            color: Colors.white,
            size: 24,
          ),
          title: SelectableText(
            translator.discord!,
            style: textTheme.bodyMedium?.copyWith(color: colorScheme.secondary),
          ),
        ),
        const Divider(thickness: 1),
      ],
    );
  }

  Widget _buildContactsList(TextTheme textTheme) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        ...translator.contacts.map((contact) {
          final isLink = _isLink(contact[1]);
          return Padding(
            padding: const EdgeInsets.symmetric(vertical: 6),
            child: ListTile(
              leading: Icon(
                isLink ? Symbols.link : Symbols.person,
                color: Colors.grey,
              ),
              title: Text(contact[0], style: textTheme.bodyMedium),
              subtitle:
                  isLink
                      ? Hyperlink(title: contact[0], link: contact[1])
                      : SelectableText(contact[1], style: textTheme.bodyMedium),
            ),
          );
        }),
      ],
    );
  }

  Widget _buildCloseButton(BuildContext context) {
    return Positioned(
      top: 16,
      right: 16,
      child: IconButton(
        icon: const Icon(Symbols.close),
        onPressed: () => Navigator.of(context).pop(),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Translator>('translator', translator));
  }
}
