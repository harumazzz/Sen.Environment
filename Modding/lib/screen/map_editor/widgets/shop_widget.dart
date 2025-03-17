import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../../../extension/platform.dart';
import '../bloc/section/section_bloc.dart';
import 'shop_tab/animation_shop.dart';
import 'shop_tab/event_shop.dart';
import 'shop_tab/image_shop.dart';

class ShopWidget extends StatelessWidget {
  const ShopWidget({super.key});

  @override
  Widget build(BuildContext context) {
    final isLowScreenWidth = MediaQuery.sizeOf(context).width < 1540;
    final isDesktopPlatform = CurrentPlatform.isDesktop;
    return BlocBuilder<SectionBloc, SectionState>(
      buildWhen:
          (prev, state) =>
              prev.section != state.section ||
              prev.sectionMinize != state.sectionMinize,
      builder: (context, state) {
        final sectionType = state.section;
        const emptyWidget = SizedBox.expand();
        if (isDesktopPlatform) {
          return Positioned(
            bottom: 20,
            left: 20,
            child: SizedBox(
              width: isLowScreenWidth ? 500 : 800,
              height: 400,
              child: switch (sectionType) {
                SectionType.select => emptyWidget,
                SectionType.image =>
                  !state.sectionMinize[SectionType.image]!
                      ? const ImageShopView()
                      : emptyWidget,
                SectionType.animation =>
                  !state.sectionMinize[SectionType.animation]!
                      ? const AnimationShopView()
                      : emptyWidget,
                SectionType.event =>
                  !state.sectionMinize[SectionType.event]!
                      ? const EventShopView()
                      : emptyWidget,
              },
            ),
          );
        } else {
          return Center(
            child: Padding(
              padding: const EdgeInsets.all(18),
              child: Column(
                mainAxisAlignment: MainAxisAlignment.end,
                children: [
                  if (sectionType == SectionType.image &&
                      !state.sectionMinize[SectionType.image]!)
                    const SizedBox(height: 400, child: ImageShopView()),
                  if (sectionType == SectionType.animation &&
                      !state.sectionMinize[SectionType.animation]!)
                    const SizedBox(height: 400, child: AnimationShopView()),
                  if (sectionType == SectionType.event &&
                      !state.sectionMinize[SectionType.event]!)
                    const SizedBox(height: 400, child: EventShopView()),
                ],
              ),
            ),
          );
        }
      },
    );
  }
}
