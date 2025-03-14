import 'dart:collection';

import 'package:flutter/material.dart';

enum LockProperties { visible, lockVisible, opacityVisible, invisible }

class ItemProperty {
  ItemProperty({this.status = LockProperties.visible});
  LockProperties status;

  ItemProperty clone() {
    return ItemProperty(status: status);
  }
}

enum NodeType { root, parent, item }

class LayerNode {
  LayerNode({
    required this.title,
    required this.icon,
    required this.type,
    this.iconExpanded,
    SplayTreeMap<int, LayerNode>? children,
    // ItemProperty? property,
    List<(IconData, dynamic Function())>? actionList,
    List<bool Function()>? onActionAccepted,
  }) : children = children ?? SplayTreeMap((a, b) => b - a),
       //  property = property ?? ItemProperty(),
       actionList = actionList ?? [],
       onActionAccepted = onActionAccepted ?? [];

  String title;
  // final ItemProperty property;
  final NodeType type;
  final IconData icon;
  final IconData? iconExpanded;
  final SplayTreeMap<int, LayerNode> children;
  final List<(IconData, dynamic Function())> actionList;
  final List<bool Function()> onActionAccepted;

  LayerNode copyWith({
    String? title,
    NodeType? type,
    IconData? icon,
    IconData? iconExpanded,
    SplayTreeMap<int, LayerNode>? children,
    //  ItemProperty? property,
    List<(IconData, dynamic Function())>? actionList,
    List<bool Function()>? onActionAccepted,
  }) {
    final newChildren = SplayTreeMap<int, LayerNode>();
    final currentChildren = children ?? this.children;
    for (final childKeys in currentChildren.keys) {
      newChildren[childKeys] = currentChildren[childKeys]!.copyWith();
    }
    return LayerNode(
      title: title ?? this.title,
      type: type ?? this.type,
      icon: icon ?? this.icon,
      iconExpanded: iconExpanded ?? this.iconExpanded,
      children: newChildren,
      actionList: (actionList ?? this.actionList).map((e) => e).toList(),
      onActionAccepted:
          (onActionAccepted ?? this.onActionAccepted).map((e) => e).toList(),
    );
  }
}
