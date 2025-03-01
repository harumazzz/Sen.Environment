import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/extension/context.dart';

class ThemeOptionList extends StatelessWidget {
  const ThemeOptionList({
    super.key,
  });

  Map<String, String> themeOf(BuildContext context) {
    return {
      'system': context.los.system,
      'light': context.los.light,
      'dark': context.los.dark,
    };
  }

  @override
  Widget build(BuildContext context) {
    final themes = themeOf(context);
    return Column(
      mainAxisSize: MainAxisSize.min,
      children: [
        ...themes.entries.map(
          (entry) => RadioListTile<String>(
            title: Text(entry.value),
            value: entry.key,
            groupValue: context.watch<SettingsCubit>().state.theme,
            onChanged: (String? theme) async {
              if (theme == null) return;
              await context.read<SettingsCubit>().setTheme(theme);
            },
          ),
        ),
      ],
    );
  }
}
