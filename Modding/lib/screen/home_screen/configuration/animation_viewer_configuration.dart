import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../../cubit/settings_cubit/settings_cubit.dart';
import '../../../i18n/app_localizations.dart';

class AnimationViewerConfiguration extends StatelessWidget {
  const AnimationViewerConfiguration({super.key});

  void _onChange(bool? value, BuildContext context) async {
    if (value == null) {
      return;
    }
    await BlocProvider.of<SettingsCubit>(
      context,
    ).setShowAnimationViewerLabel(value: value);
  }

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    return Column(
      mainAxisSize: MainAxisSize.min,
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        SwitchListTile(
          contentPadding: EdgeInsets.zero,
          title: Text(
            los.show_label,
            style: Theme.of(context).textTheme.titleMedium,
            maxLines: 4,
            overflow: TextOverflow.ellipsis,
          ),
          subtitle: Text(
            los.show_label_description,
            style: Theme.of(context).textTheme.labelMedium,
            maxLines: 4,
            overflow: TextOverflow.ellipsis,
          ),
          value: context.watch<SettingsCubit>().state.showAnimationViewerLabel,
          onChanged: (value) => _onChange(value, context),
        ),
      ],
    );
  }
}
