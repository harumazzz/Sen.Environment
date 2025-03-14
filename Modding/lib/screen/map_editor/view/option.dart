import 'package:flutter/widgets.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import '../bloc/history/history_bloc.dart';
import '../bloc/item/item_bloc.dart';
import '../bloc/section/section_bloc.dart';
import '../bloc/selected/selected_bloc.dart';
import '../widgets/option_tab/event_config.dart';
import '../widgets/option_tab/map_config.dart';
import '../widgets/option_tab/island_config.dart';

class OptionTabView extends StatelessWidget {
  const OptionTabView({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<SectionBloc, SectionState>(
      buildWhen: (prev, state) => prev.section != state.section,
      builder: (context, state) {
        final selectedTab = state.section;
        return BlocBuilder<HistoryBloc, HistoryState>(
          buildWhen: (prev, state) => prev.index != state.index,
          builder: (context, state) {
            return BlocBuilder<SelectedBloc, SelectedState>(
              builder: (context, state) {
                final id = state.selectedList.lastOrNull;
                final itemStore = context.read<ItemBloc>().state.itemStore;
                if (selectedTab == SectionType.select ||
                    itemStore[id] == null) {
                  return const MapConfigTab();
                } else {
                  final itemProfile = itemStore[id]!;
                  if (itemProfile.isEvent) {
                    return EventConfigTab(id: id!);
                  } else {
                    return IslandConfigTab(id: id!);
                  }
                }
              },
            );
          },
        );
      },
    );
  }
}
