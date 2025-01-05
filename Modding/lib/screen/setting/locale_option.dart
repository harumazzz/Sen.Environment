import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';

@immutable
class LocaleOption extends StatelessWidget {
  const LocaleOption({
    super.key,
    required this.title,
    required this.value,
  });

  final String title;

  final String value;

  void _onChange(
    String? value,
    BuildContext context,
  ) async {
    if (value == null) return;
    await BlocProvider.of<SettingsCubit>(context).setLocale(value);
  }

  @override
  Widget build(BuildContext context) {
    return ListTile(
      title: Text(title),
      leading: BlocBuilder<SettingsCubit, SettingsState>(
        builder: (context, state) => Radio<String>(
          value: value,
          groupValue: state.locale,
          onChanged: (String? value) => _onChange(value, context),
        ),
      ),
    );
  }
}
