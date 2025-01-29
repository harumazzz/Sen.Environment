import 'dart:collection';
import 'package:flutter/material.dart';

class CaptureManager<T> {
  /// Creates an undo/redo manager with an optional memory limit.
  ///
  /// If [maxMemory] is specified, it limits the number of states stored in the
  /// undo stack. If `null`, the memory is unlimited.
  ///
  /// Parameters:
  /// - [maxMemory]: The maximum number of states to retain in memory for undo/redo.
  CaptureManager({int? maxMemory}) : _maxMemory = maxMemory ?? 50;

  final int _maxMemory;

  // final Map<dynamic, String> _locale;

  final Queue<T> _stackList = Queue();

  List<T> get stackList => _stackList.toList();

  // final Queue<String> _captureLog = Queue();

  // List<String> get captureLog => _captureLog.toList();

  /// Returns the number of states stored in the undo stack.
  int get length => _stackList.length;

  /// A notifier that emits the current index of the undo/redo stack.
  final ValueNotifier<int> notifier = ValueNotifier<int>(-1);

  /*
  String? getLogByIndex(int index) {
    return _captureLog.elementAtOrNull(index);
  }
  */

  /// Checks if there is an action available to redo.
  ///
  /// Returns:
  /// - `true` if there are states in the redo stack that can be redone.
  /// - Otherwise, returns `false`.
  bool canUndo() {
    return _stackList.isNotEmpty && notifier.value > 0;
  }

  /// Checks if there is an action available to undo.
  ///
  /// Returns:
  /// - `true` if there is more than one state in the undo stack, indicating that
  /// an undo operation is possible.
  /// - Otherwise, returns `false`.
  bool canRedo() {
    return _stackList.isNotEmpty && notifier.value < length - 1;
  }

  /// Undoes the last action and returns the previous state if available.
  ///
  /// If there is a state to undo, it will be removed from the undo stack and added to
  /// the redo stack. The previous state before the undone action will be returned.
  ///
  /// Returns:
  /// - Undone state [T].
  /// - `null` if there is no state to undo.
  T? undo() {
    if (canUndo()) {
      return _stackList.elementAtOrNull(--notifier.value);
    }
    return null;
  }

  /// Redoes the last undone action and returns the current state.
  ///
  /// If there is a state to redo, it will be removed from the redo stack and added back
  /// to the undo stack. The redone action will be returned.
  ///
  /// Returns:
  /// - Redone state [T].
  /// - `null` if there is no state to redo.
  T? redo() {
    if (canRedo()) {
      return _stackList.elementAtOrNull(++notifier.value);
    }
    return null;
  }

  /// Initializes the undo/redo manager with an initial state.
  ///
  /// This method should be called to set up the initial state before any undo or redo
  /// operations are performed.
  ///
  /// Requires:
  /// - [state]: The initial action to initialize the undo/redo stack with. This must be passed by value.
  void initialize(T state) {
    if (_stackList.isNotEmpty) {
      dispose();
    }
    captureState(state);
  }

  /// Adds the state to the undo stack.
  ///
  /// This method should be called every time an action is performed. It clears the redo
  /// stack to ensure that redo operations are only available for actions that have been undone.
  ///
  /// If [maxMemory] is specified, the oldest states are removed to stay within the memory limit.
  ///
  /// Requires:
  /// - [state]: The current state to be added to the undo stack. This must be passed by value.
  void captureState(T state) {
    if (length > _maxMemory) {
      _stackList.removeFirst();
    }
    while (canRedo()) {
      _stackList.removeLast();
    }
    _stackList.add(state);
    if (notifier.value < _maxMemory) {
      ++notifier.value;
    } else {
      //To fix value listener
      --notifier.value;
      ++notifier.value;
    }
    return;
  }

  /// Disposes the manager by clearing both undo and redo stacks.
  ///
  /// This method is used to clean up resources when the undo/redo manager is no longer needed.
  void dispose() {
    _stackList.clear();
    notifier.value = -1;
  }
}
