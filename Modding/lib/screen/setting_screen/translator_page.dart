import 'package:flutter/material.dart';
import 'package:flutter/foundation.dart';
import 'package:material_symbols_icons/symbols.dart';

import '../../extension/context.dart';
import '../../model/translator.dart';
import '../../widget/hyperlink.dart';

class TranslatorPage extends StatelessWidget {
  const TranslatorPage({super.key, required this.translator});

  final Translator translator;

  @override
  Widget build(BuildContext context) {
    return Stack(
      children: [
        TranslatorCard(translator: translator),
        const CloseButtonOverlay(),
      ],
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Translator>('translator', translator));
  }
}

class TranslatorCard extends StatelessWidget {
  const TranslatorCard({super.key, required this.translator});

  final Translator translator;

  @override
  Widget build(BuildContext context) {
    return Card(
      elevation: 3,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16.0)),
      child: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.symmetric(vertical: 16.0, horizontal: 24.0),
          child: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              ProfileSection(translator: translator),
              const SizedBox(height: 16.0),
              const Divider(thickness: 1.0),
              if (translator.discord != null)
                DiscordTile(discord: translator.discord!),
              ContactsList(contacts: translator.contacts),
            ],
          ),
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Translator>('translator', translator));
  }
}

class ProfileSection extends StatelessWidget {
  const ProfileSection({super.key, required this.translator});

  final Translator translator;

  @override
  Widget build(BuildContext context) {
    final colorScheme = Theme.of(context).colorScheme;
    final textTheme = Theme.of(context).textTheme;

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

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(DiagnosticsProperty<Translator>('translator', translator));
  }
}

class DiscordTile extends StatelessWidget {
  const DiscordTile({super.key, required this.discord});

  final String discord;

  @override
  Widget build(BuildContext context) {
    final colorScheme = Theme.of(context).colorScheme;
    final textTheme = Theme.of(context).textTheme;

    return Column(
      children: [
        ListTile(
          leading: const Icon(
            Icons.discord_outlined,
            color: Colors.white,
            size: 24,
          ),
          title: SelectableText(
            discord,
            style: textTheme.bodyMedium?.copyWith(color: colorScheme.secondary),
          ),
        ),
        const Divider(thickness: 1),
      ],
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(StringProperty('discord', discord));
  }
}

class ContactsList extends StatelessWidget {
  const ContactsList({super.key, required this.contacts});

  final List<List<String>> contacts;

  bool _isLink(String value) {
    final urlRegex = RegExp(
      r'((([A-Za-z]{3,9}:(?:\/\/)?)(?:[-;:&=+\$,\w]+@)?[A-Za-z0-9.-]+|(?:www.|[-;:&=+\$,\w]+@)[A-Za-z0-9.-]+)((?:\/[\+~%\/.\w-_]*)?\??(?:[-\+=&;%@.\w_]*)#?(?:[\w]*))?)',
    );
    return urlRegex.hasMatch(value);
  }

  @override
  Widget build(BuildContext context) {
    final textTheme = Theme.of(context).textTheme;

    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        ...contacts.map((contact) {
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

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(IterableProperty<List<String>>('contacts', contacts));
  }
}

class CloseButtonOverlay extends StatelessWidget {
  const CloseButtonOverlay({super.key});

  @override
  Widget build(BuildContext context) {
    return Positioned(
      top: 16,
      right: 16,
      child: IconButton(
        icon: const Icon(Symbols.close),
        onPressed: () => Navigator.of(context).pop(),
      ),
    );
  }
}
