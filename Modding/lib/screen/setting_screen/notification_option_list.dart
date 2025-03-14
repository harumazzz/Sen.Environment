import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../cubit/settings_cubit/settings_cubit.dart';
import '../../extension/context.dart';

class NotificationOptionList extends StatelessWidget {
  const NotificationOptionList({super.key});

  Future<void> _onNotificationChanged(BuildContext context, bool? value) async {
    if (value == null) {
      return;
    }
    await context.read<SettingsCubit>().setNotification(
      sendNotification: value,
    );
  }

  @override
  Widget build(BuildContext context) {
    final isEnabled = context.watch<SettingsCubit>().state.sendNotification;
    return Column(
      mainAxisSize: MainAxisSize.min,
      children: [
        RadioListTile<bool>(
          title: Text(context.los.enable),
          value: true,
          groupValue: isEnabled,
          onChanged: (value) => _onNotificationChanged(context, value),
        ),
        RadioListTile<bool>(
          title: Text(context.los.disable),
          value: false,
          groupValue: isEnabled,
          onChanged: (value) => _onNotificationChanged(context, value),
        ),
      ],
    );
  }
}
