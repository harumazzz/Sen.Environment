import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'painter.dart';
import '../models/item_profile.dart';

class MultiSelectBox extends StatelessWidget {
  const MultiSelectBox({
    super.key,
    required this.idList,
    required this.itemStore,
  });

  final List<String> idList;

  final Map<String, ItemProfile> itemStore;

  @override
  Widget build(BuildContext context) {
    final stackList = <Widget>[];
    idList.retainWhere(itemStore.containsKey);
    for (final id in idList) {
      if (itemStore.containsKey(id)) {
        final itemProfile = itemStore[id]!;
        stackList.add(SelectedBox(id: id, itemProfile: itemProfile));
      }
    }

    return Stack(fit: StackFit.passthrough, children: stackList);
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(IterableProperty<String>('idList', idList));
    properties.add(
      DiagnosticsProperty<Map<String, ItemProfile>>('itemStore', itemStore),
    );
  }
}

class SelectedBox extends StatelessWidget {
  const SelectedBox({super.key, required this.id, required this.itemProfile});

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
        child: Visibility(visible: false, child: itemProfile.widget!),
      );
    } else {
      return CustomPaint(
        foregroundPainter: BorderPainer(
          itemRect: itemProfile.itemRect!,
          matrix: itemProfile.matrix,
          borderColor: Colors.yellow,
          borderWidth: 10.0,
        ),
        child: Visibility(visible: false, child: itemProfile.widget!),
      );
    }
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(
      DiagnosticsProperty<ItemProfile>('itemProfile', itemProfile),
    );
    properties.add(StringProperty('id', id));
  }
}
