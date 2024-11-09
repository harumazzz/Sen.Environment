import 'package:flutter/material.dart';

class LevelLawn extends StatefulWidget {
  const LevelLawn({
    super.key,
    required this.selectedItem,
    required this.onTap,
  });

  final MemoryImage? selectedItem;

  final void Function(int row, int col) onTap;

  @override
  State<LevelLawn> createState() => _LevelLawnState();
}

class _LevelLawnState extends State<LevelLawn> {
  final int rows = 5;
  final int columns = 9;

  final Map<int, Widget> cellItems = {};

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      height: 300,
      width: 500,
      child: GridView.builder(
        gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: columns,
          mainAxisSpacing: 2,
          crossAxisSpacing: 2,
        ),
        itemCount: rows * columns,
        itemBuilder: (context, index) {
          final row = index ~/ columns;
          final col = index % columns;

          return GestureDetector(
            onTap: () {
              setState(
                () {
                  widget.onTap(row, col);
                  if (widget.selectedItem != null &&
                      !cellItems.containsKey(index)) {
                    cellItems[index] = SizedBox(
                      width: 40,
                      height: 40,
                      child: Image(
                        image: widget.selectedItem!,
                        width: 40,
                        height: 40,
                      ),
                    );
                  } else {
                    cellItems.remove(index);
                  }
                },
              );
            },
            child: Container(
              color: Colors.white,
              child: Center(
                child: cellItems.containsKey(index)
                    ? cellItems[index]
                    : const SizedBox.shrink(),
              ),
            ),
          );
        },
      ),
    );
  }
}
