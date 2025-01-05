import 'dart:ffi';

import 'package:ffi/ffi.dart';
import 'package:sen/model/api.dart';
import 'package:sen/service/pointer_helper.dart';

class KernelArgument {
  final String currentShell;
  final String kernelPath;
  final String scriptPath;
  final List<String> arguments;
  final Pointer<CStringList> value;

  KernelArgument(
    this.currentShell,
    this.kernelPath,
    this.scriptPath,
    this.arguments,
  ) : value = calloc<CStringList>() {
    final argumentList = [
      currentShell,
      kernelPath,
      scriptPath,
      ...arguments,
    ];
    _allocateFull(argumentList);
  }

  void _allocateFull(List<String> arguments) {
    final argumentPointer = calloc<CStringView>(arguments.length);
    for (var i = 0; i < arguments.length; ++i) {
      final currentArgument = PointerHelper.toUint8List(arguments[i]);
      final currentArgumentPointer = PointerHelper.utf8ListToCString(currentArgument);
      (argumentPointer + i).ref
        ..size = currentArgument.length
        ..value = currentArgumentPointer;
    }
    value.ref
      ..size = arguments.length
      ..value = argumentPointer;
  }

  void cleanup() {
    for (var i = 0; i < arguments.length; ++i) {
      calloc.free((value.ref.value + i).ref.value.cast<Int8>());
    }
    calloc.free(value.ref.value);
    calloc.free(value);
  }
}
