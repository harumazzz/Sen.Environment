import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:sen/provider/setting_provider.dart';

@immutable
class ThemeOption extends ConsumerWidget {
  const ThemeOption({
    super.key,
    required this.title,
    required this.value,
  });

  final String title;

  final String value;

  void _onChange(String? value, WidgetRef ref) {
    if (value == null) return;
    ref.watch(settingProvider.notifier).setTheme(value);
  }

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return ListTile(
      title: Text(title),
      leading: Radio<String>(
        value: value,
        groupValue: ref.read(settingProvider).theme,
        onChanged: (String? value) => _onChange(value, ref),
      ),
    );
  }
}
