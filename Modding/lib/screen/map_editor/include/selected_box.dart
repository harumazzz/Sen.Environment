import 'package:flutter/material.dart';
import 'package:sen/screen/map_editor/include/painter.dart';
import 'package:sen/screen/map_editor/models/item_profile.dart';

class MultiSelectBox extends StatelessWidget {
  const MultiSelectBox({super.key, required this.idList, required this.itemStore});

  final List<String> idList;

  final Map<String, ItemProfile> itemStore;

  @override
  Widget build(BuildContext context) {
    final stackList = <Widget>[];
    idList.retainWhere((e) => itemStore.containsKey(e));
    for (final id in idList) {
      if (itemStore.containsKey(id)) {
        final itemProfile = itemStore[id]!;
        stackList.add(SelectedBox(
          id: id,
          itemProfile: itemProfile,
        ));
      }
    }

    return Stack(
      fit: StackFit.passthrough,
      children: stackList,
    );
  }
}

class SelectedBox extends StatelessWidget {
  const SelectedBox({
    super.key,
    required this.id,
    required this.itemProfile,
  });

  final String id;

  final ItemProfile itemProfile;

  @override
  Widget build(Object context) {
    if (itemProfile.isEvent) {
      return CustomPaint(
        foregroundPainter: BorderPainer(
          itemRect: itemProfile.itemRect!,
          matrix: itemProfile.matrix,
          borderColor: Colors.yellow,
          borderWidth: 10.0,
        ),
        child: Visibility(
          visible: false,
          child: itemProfile.widget!,
        ),
      );
    } else {
      return CustomPaint(
        foregroundPainter: BorderPainer(
            itemRect: itemProfile.itemRect!, matrix: itemProfile.matrix, borderColor: Colors.yellow, borderWidth: 10.0),
        child: Visibility(
          visible: false,
          child: itemProfile.widget!,
        ),
      );
    }
  }
}
