extension ListExtension<T> on List<T> {
  void removeRangeIf(
    int start,
    int end,
    bool Function(T element) shouldRemove,
  ) {
    if (start < 0 || end > length || start > end) {
      throw RangeError('Invalid range: start=$start, end=$end, length=$length');
    }
    final newList = <T>[];
    for (var i = 0; i < length; i++) {
      if (i < start || i >= end || !shouldRemove(this[i])) {
        newList.add(this[i]);
      }
    }
    clear();
    addAll(newList);
  }

  void removeRangeIfIndex(
    int start,
    int end,
    bool Function(int index) shouldRemove,
  ) {
    if (start < 0 || end > length || start > end) {
      throw RangeError('Invalid range: start=$start, end=$end, length=$length');
    }
    final newList = <T>[];
    for (var i = 0; i < length; i++) {
      if (i < start || i >= end || !shouldRemove(i)) {
        newList.add(this[i]);
      }
    }
    clear();
    addAll(newList);
  }
}
