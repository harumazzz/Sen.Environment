import 'package:flutter/material.dart';

class ModernContextMenu extends StatelessWidget {
  final List<MenuItem> items;
  final Function(String) onSelect;

  const ModernContextMenu({
    Key? key,
    required this.items,
    required this.onSelect,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Dialog(
      backgroundColor: Theme.of(context).colorScheme.onSurfaceVariant,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: items.map((item) => _buildMenuItem(context, item)).toList(),
        ),
      ),
    );
  }

  Widget _buildMenuItem(BuildContext context, MenuItem item) {
    return MenuAnchor(
      builder: (context, controller, child) {
        return ListTile(
          title: Text(item.title),
          trailing:
              item.subItems.isNotEmpty ? Icon(Icons.arrow_drop_down) : null,
          onTap: () {
            if (item.subItems.isNotEmpty) {
              controller.open();
            } else {
              onSelect(item.title);
            }
          },
        );
      },
      menuChildren:
          item.subItems.map((subItem) {
            return MenuItemButton(
              child: Text(subItem.title),
              onPressed: () => onSelect(subItem.title),
            );
          }).toList(),
    );
  }
}

class MenuItem {
  final String title;
  final List<MenuItem> subItems;

  MenuItem({required this.title, this.subItems = const []});
}
