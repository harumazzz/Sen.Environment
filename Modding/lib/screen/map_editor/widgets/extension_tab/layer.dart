import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:material_symbols_icons/symbols.dart';
import 'package:nil/nil.dart';
import 'package:pie_menu/pie_menu.dart';
import '../../../../cubit/map_editor_configuration_cubit/map_editor_configuration_cubit.dart';
import '../../../../extension/context.dart';
import '../../bloc/item/item_bloc.dart';
import '../../bloc/layer/layer_bloc.dart';
import '../../bloc/section/section_bloc.dart';
import 'package:flutter_fancy_tree_view/flutter_fancy_tree_view.dart';
import '../../models/layer_node.dart';

class LayerWidget extends StatelessWidget {
  const LayerWidget({super.key});

  @override
  Widget build(BuildContext context) {
    final item =
        context
            .read<MapEditorConfigurationCubit>()
            .state
            .extensionItem[ExtensionType.layer]!;
    return SizedBox(
      width: 330,
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
                            type: ExtensionType.layer,
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
                  child: const Padding(
                    padding: EdgeInsets.symmetric(vertical: 10),
                    child: LayerTreeView(),
                  ),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

class LayerTreeView extends StatelessWidget {
  const LayerTreeView({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocBuilder<LayerBloc, LayerState>(
      builder: (context, state) {
        final treeController = state.treeController;
        return AnimatedTreeView<LayerNode>(
          treeController: treeController,
          nodeBuilder: (BuildContext context, TreeEntry<LayerNode> entry) {
            return TreeIndentation(
              entry: entry,
              child: Padding(
                padding: const EdgeInsets.symmetric(horizontal: 8),
                child: TreeTile(
                  entry: entry,
                  onFolderPressed: () {
                    treeController.toggleExpansion(entry.node);
                  },
                ),
              ),
            );
          },
          duration: const Duration(milliseconds: 200),
        );
      },
    );
  }
}

class TreeTile extends StatelessWidget {
  const TreeTile({super.key, required this.entry, this.onFolderPressed});

  final TreeEntry<LayerNode> entry;

  final VoidCallback? onFolderPressed;

  @override
  Widget build(BuildContext context) {
    final node = entry.node;
    final textEditing = TextEditingController(text: node.title);
    final pieActionList = <PieAction>[];
    for (var i = 0; i < node.actionList.length; ++i) {
      final action = node.actionList[i];
      if (node.onActionAccepted[i]()) {
        pieActionList.add(
          PieAction(tooltip: nil, onSelect: action.$2, child: Icon(action.$1)),
        );
      }
    }
    final moreHoriz = Tooltip(
      message: context.los.more_actions,
      waitDuration: const Duration(seconds: 1),
      child: IconButton(
        onPressed: pieActionList.isNotEmpty ? () {} : null,
        icon: const Icon(Symbols.more_horiz),
      ),
    );
    return SizedBox(
      height: 60,
      child: Card(
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(8)),
        child: switch (node.type) {
          NodeType.root => InkWell(
            borderRadius: BorderRadius.circular(8),
            onTap: onFolderPressed,
            child: Row(
              children: [
                Padding(
                  padding: const EdgeInsets.symmetric(horizontal: 12),
                  child:
                      entry.isExpanded
                          ? Icon(node.iconExpanded)
                          : Icon(node.icon),
                ),
                Expanded(child: Text(node.title)),
                Padding(
                  padding: const EdgeInsets.symmetric(horizontal: 8),
                  child: PieMenu(
                    onPressed: () {},
                    actions: pieActionList,
                    child: moreHoriz,
                  ),
                ),
              ],
            ),
          ),
          NodeType.parent => InkWell(
            borderRadius: BorderRadius.circular(8),
            onTap: onFolderPressed,
            child: Row(
              children: [
                Padding(
                  padding: const EdgeInsets.symmetric(horizontal: 12),
                  child:
                      entry.isExpanded
                          ? Icon(node.iconExpanded)
                          : Icon(node.icon),
                ),
                SizedBox(
                  width: 80,
                  child: TextFormField(
                    controller: textEditing,
                    style: Theme.of(context).textTheme.titleSmall,
                    decoration: const InputDecoration(
                      border: InputBorder.none,
                      contentPadding: EdgeInsets.fromLTRB(12.0, 4.0, 0.0, 12.0),
                    ),
                    autovalidateMode: AutovalidateMode.always,
                    onFieldSubmitted: (value) {
                      if (!context.read<LayerBloc>().isVaildLayerName(value)) {
                        textEditing.text = node.title;
                      } else {
                        node.title = value;
                        context.read<ItemBloc>().add(const ItemStoreUpdated());
                      }
                    },
                  ),
                ),
                const Spacer(),
                Padding(
                  padding: const EdgeInsets.symmetric(horizontal: 8),
                  child: PieMenu(
                    onPressed: () {},
                    actions: pieActionList,
                    child: moreHoriz,
                  ),
                ),
              ],
            ),
          ),
          NodeType.item => Row(
            children: [
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 12),
                child: Icon(node.icon),
              ),
              Expanded(child: Text(node.title)),
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 8),
                child: PieMenu(
                  onPressed: () {},
                  actions: pieActionList,
                  child: moreHoriz,
                ),
              ),
            ],
          ),
        },
      ),
    );
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      ObjectFlagProperty<VoidCallback?>.has('onFolderPressed', onFolderPressed),
    );
    properties.add(DiagnosticsProperty<TreeEntry<LayerNode>>('entry', entry));
  }
}
