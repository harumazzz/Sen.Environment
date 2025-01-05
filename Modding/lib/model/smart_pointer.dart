import 'dart:ffi';

import 'package:ffi/ffi.dart';

class SmartPointer<T extends NativeType> {
  final Pointer<T> _value;

  final Finalizer _finalizer;

  SmartPointer(this._value)
      : _finalizer = Finalizer<Pointer<T>>(
          (Pointer<T> pointer) => calloc.free(pointer),
        ) {
    _finalizer.attach(this, _value);
  }

  Pointer<T> get value {
    if (_value == nullptr) {
      throw Exception('value is nullptr');
    }
    return _value;
  }

  factory SmartPointer.asNullPointer() {
    return SmartPointer(nullptr.cast<T>());
  }
}
