import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import '../../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../../bloc/history/history_bloc.dart';
import '../../bloc/section/section_bloc.dart';
import '../../models/action_service.dart';

class HistoryWidget extends StatelessWidget {
  const HistoryWidget({super.key});

  @override
  Widget build(BuildContext context) {
    final item =
        context
            .read<MapEditorConfigurationCubit>()
            .state
            .extensionItem[ExtensionType.history]!;

    return SizedBox(
      width: 300,
      height: 400,
      child: Card(
        color: Theme.of(context).colorScheme.surface,
        child: Padding(
          padding: const EdgeInsets.only(
            top: 4,
            right: 16,
            left: 16,
            bottom: 16,
          ),
          child: Column(
            children: [
              SizedBox(
                height: 40,
                child: Row(
                  children: [
                    Container(
                      margin: const EdgeInsets.symmetric(horizontal: 10),
                      child: Icon(item.icon),
                    ),
                    Text(
                      item.title,
                      style: const TextStyle(fontWeight: FontWeight.bold),
                    ),
                    const Spacer(),
                    IconButton(
                      onPressed: () {
                        context.read<SectionBloc>().add(
                          const ExtensionToggled(
                            type: ExtensionType.history,
                            enabled: false,
                          ),
                        );
                      },
                      icon: const Icon(Symbols.close),
                    ),
                  ],
                ),
              ),
              Expanded(
                child: Container(
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.circular(12),
                    color: Theme.of(context).colorScheme.secondaryContainer,
                  ),
                  child: const HistoryTree(),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

class HistoryTree extends StatelessWidget {
  const HistoryTree({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<HistoryBloc, HistoryState>(
      builder: (context, state) {
        final captureManager = state.captureManager;
        final scrollController = state.scrollController;
        return Padding(
          padding: const EdgeInsets.symmetric(vertical: 8),
          child: ValueListenableBuilder(
            valueListenable: captureManager.notifier,
            builder: (context, notifierIndex, ref) {
              final stackList = captureManager.stackList;
              WidgetsBinding.instance.addPostFrameCallback((_) {
                if (captureManager.canUndo() && !captureManager.canRedo()) {
                  scrollController.jumpTo(
                    scrollController.position.maxScrollExtent,
                  );
                }
              });
              return ListView.builder(
                controller: scrollController,
                itemCount: stackList.length,
                itemBuilder: (context, index) {
                  final actionService = stackList[index];
                  return Padding(
                    padding: const EdgeInsets.symmetric(horizontal: 8),
                    child: HistoryTile(
                      actionService: actionService,
                      notifierIndex: notifierIndex,
                      index: index,
                    ),
                  );
                },
              );
            },
          ),
        );
      },
    );
  }
}

class HistoryTile extends StatelessWidget {
  const HistoryTile({
    super.key,
    required this.actionService,
    required this.notifierIndex,
    required this.index,
  });
  final ActionService actionService;
  final int notifierIndex;
  final int index;

  @override
  Widget build(BuildContext context) {
    final colorScheme = Theme.of(context).colorScheme;
    Color color = colorScheme.surfaceContainerLow;
    if (notifierIndex == index) {
      color =
          Theme.of(context).brightness == Brightness.dark
              ? colorScheme.onPrimaryFixedVariant
              : colorScheme.primaryFixedDim;
    } else if (notifierIndex < index) {
      color = colorScheme.outlineVariant;
    }
    final actionTypeLocalization =
        context
            .read<MapEditorConfigurationCubit>()
            .state
            .actionTypeLocalization;
    return Card(
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(8)),
      color: color,
      child: InkWell(
        borderRadius: BorderRadius.circular(8),
        onTap:
            notifierIndex != index
                ? () {
                  if (index < notifierIndex) {
                    for (var i = 0; i < notifierIndex - index; ++i) {
                      context.read<HistoryBloc>().add(const UndoEvent());
                    }
                  } else {
                    for (var i = 0; i < index - notifierIndex; ++i) {
                      context.read<HistoryBloc>().add(const RedoEvent());
                    }
                  }
                }
                : null,
        child: ListTile(
          dense: true,
          leading: Icon(
            context.read<HistoryBloc>().getIconByType(actionService.actionType),
          ),
          title: Text(
            actionTypeLocalization[actionService.actionType] ?? 'null',
            style: Theme.of(context).textTheme.titleSmall,
          ),
        ),
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<ActionService>('actionService', actionService),
    );
    properties.add(IntProperty('notifierIndex', notifierIndex));
    properties.add(IntProperty('index', index));
  }
}
