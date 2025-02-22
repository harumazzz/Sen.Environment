import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/i18n/app_localizations.dart';
import 'package:sen/cubit/settings_cubit/settings_cubit.dart';
import 'package:sen/model/item.dart';
import 'package:sen/screen/home/home_screen.dart';

class OptionList extends StatelessWidget {
  const OptionList({
    super.key,
    required this.items,
  });

  final List<Item> items;

  @override
  Widget build(BuildContext context) {
    final los = AppLocalizations.of(context)!;
    final isDesktop = Platform.isMacOS || Platform.isLinux || Platform.isWindows;
    final settings = BlocProvider.of<SettingsCubit>(context, listen: true);
    return isDesktop ? _buildGridView(context, settings, los) : _buildListView(settings, los);
  }

  Widget _buildGridView(
    BuildContext context,
    SettingsCubit settings,
    AppLocalizations los,
  ) {
    final screenWidth = MediaQuery.of(context).size.width;
    const itemWidth = 250.0;
    final crossAxisCount = (screenWidth / itemWidth).floor();

    return GridView.builder(
      gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
        crossAxisCount: crossAxisCount,
        childAspectRatio: 1.0,
      ),
      itemCount: items.length,
      itemBuilder: (context, index) {
        final item = items[index];
        return _buildCard(item, settings, los, true);
      },
    );
  }

  Widget _buildListView(
    SettingsCubit settings,
    AppLocalizations los,
  ) {
    return ListView.builder(
      itemCount: items.length,
      itemBuilder: (context, index) {
        final item = items[index];
        return _buildCard(item, settings, los, false);
      },
    );
  }

  Widget _buildCard(
    Item item,
    SettingsCubit settings,
    AppLocalizations los,
    bool isDesktop,
  ) {
    if (item.isDisabled) {
      return const SizedBox.shrink();
    }
    return isDesktop
        ? GridCard(
            item: item,
            isValid: settings.state.isValid,
            invalidMessage: los.toolchain_is_invalid,
            onSetting: item.onSetting,
          )
        : ListCard(
            item: item,
            isValid: settings.state.isValid,
            invalidMessage: los.toolchain_is_invalid,
            onSetting: item.onSetting,
          );
  }
}
