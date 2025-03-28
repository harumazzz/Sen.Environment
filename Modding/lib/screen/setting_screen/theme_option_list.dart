import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../bloc/settings_bloc/settings_bloc.dart';
import '../../extension/context.dart';

class ThemeOptionList extends StatelessWidget {
  const ThemeOptionList({super.key});

  Map<String, String> themeOf(BuildContext context) {
    return {
      'system': context.los.system,
      'light': context.los.light,
      'dark': context.los.dark,
    };
  }

  @override
  Widget build(BuildContext context) {
    final themes = themeOf(context).entries.toList();
    final currentTheme = context.watch<SettingsBloc>().state.theme;
    return ListView.builder(
      shrinkWrap: true,
      itemCount: themes.length,
      itemBuilder: (context, index) {
        final entry = themes[index];
        return RadioListTile<String>(
          contentPadding: EdgeInsets.zero,
          title: Text(
            entry.value,
            style: Theme.of(context).textTheme.labelLarge,
          ),
          value: entry.key,
          groupValue: currentTheme,
          onChanged: (String? theme) async {
            if (theme == null) {
              return;
            }
            context.read<SettingsBloc>().add(SetTheme(theme));
          },
        );
      },
    );
  }
}
