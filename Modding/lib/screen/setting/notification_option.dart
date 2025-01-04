import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:sen/provider/setting_provider.dart';

@immutable
class NotificationOption extends ConsumerWidget {
  const NotificationOption({
    super.key,
    required this.title,
    required this.value,
  });

  final String title;

  final bool value;

  void _onChange(bool? value, WidgetRef ref) {
    if (value == null) return;
    ref.watch(settingProvider.notifier).setNotification(value);
  }

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return ListTile(
      title: Text(title),
      leading: Radio<bool>(
        value: value,
        groupValue: ref.watch(settingProvider).sendNotification,
        onChanged: (bool? value) => _onChange(value, ref),
      ),
    );
  }
}
