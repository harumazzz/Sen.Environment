import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';

@immutable
class NotificationOption extends StatelessWidget {
  const NotificationOption({
    super.key,
    required this.title,
    required this.value,
  });

  final String title;

  final bool value;

  void _onChange(
    bool? value,
    BuildContext context,
  ) async {
    if (value == null) return;
    await BlocProvider.of<SettingsCubit>(context).setNotification(value);
  }

  @override
  Widget build(BuildContext context) {
    return ListTile(
      title: Text(title),
      leading: BlocBuilder<SettingsCubit, SettingsState>(
        builder: (context, state) {
          return Radio<bool>(
            value: value,
            groupValue: state.sendNotification,
            onChanged: (bool? value) => _onChange(value, context),
          );
        },
      ),
    );
  }
}
