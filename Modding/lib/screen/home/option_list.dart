import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:flutter_gen/gen_l10n/app_localizations.dart';
import 'package:sen/model/item.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/home/home_screen.dart';

class OptionList extends ConsumerWidget {
  const OptionList({
    super.key,
    required this.items,
  });

  final List<Item> items;

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final los = AppLocalizations.of(context)!;
    final isDesktop = Platform.isMacOS || Platform.isLinux || Platform.isWindows;
    final settings = ref.watch(settingProvider);
    return isDesktop ? _buildGridView(context, settings, los) : _buildListView(settings, los);
  }

  Widget _buildGridView(BuildContext context, SettingState settings, AppLocalizations los) {
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

  Widget _buildListView(SettingState settings, AppLocalizations los) {
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
    SettingState settings,
    AppLocalizations los,
    bool isDesktop,
  ) {
    if (item.isDisabled) {
      return const SizedBox.shrink();
    }
    return isDesktop
        ? GridCard(
            item: item,
            isValid: settings.isValid,
            invalidMessage: los.toolchain_is_invalid,
            onSetting: item.onSetting,
          )
        : ListCard(
            item: item,
            isValid: settings.isValid,
            invalidMessage: los.toolchain_is_invalid,
            onSetting: item.onSetting,
          );
  }
}
