import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:sen/screen/map_editor/bloc/section/section_bloc.dart';
import 'package:sen/screen/map_editor/widgets/shop_tab/animation_shop.dart';
import 'package:sen/screen/map_editor/widgets/shop_tab/event_shop.dart';
import 'package:sen/screen/map_editor/widgets/shop_tab/image_shop.dart';

class ShopWidget extends StatelessWidget {
  const ShopWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<SectionBloc, SectionState>(
        buildWhen: (prev, state) =>
            prev.section != state.section ||
            prev.sectionMinize != state.sectionMinize,
        builder: (context, state) {
          final sectionType = state.section;
          switch (sectionType) {
            case SectionType.select:
              {
                return const SizedBox.expand();
              }
            case SectionType.image:
              {
                if (!state.sectionMinize[SectionType.image]!) {
                  return const ImageShopView();
                } else {
                  return const SizedBox.expand();
                }
              }
            case SectionType.animation:
              {
                if (!state.sectionMinize[SectionType.animation]!) {
                  return const AnimationShopView();
                } else {
                  return const SizedBox.expand();
                }
              }
            case SectionType.event:
              {
                if (!state.sectionMinize[SectionType.event]!) {
                  return const EventShopView();
                } else {
                  return const SizedBox.expand();
                }
              }
          }
        });
  }
}
