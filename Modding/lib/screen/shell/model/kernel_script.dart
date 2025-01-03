import 'dart:ffi';

import 'package:ffi/ffi.dart';
import 'package:sen/model/api.dart';
import 'package:sen/service/pointer_service.dart';

class KernelScript {
  final String scriptPath;
  final Pointer<CStringView> value;

  KernelScript(this.scriptPath) : value = calloc<CStringView>() {
    _allocateFull();
  }

  void _allocateFull() {
    final scriptNativeString = PointerService.toUint8List(scriptPath);
    final scriptPointer = PointerService.utf8ListToCString(scriptNativeString);
    value.ref
      ..value = scriptPointer
      ..size = scriptNativeString.length;
  }

  void cleanup() {
    calloc.free(value.ref.value);
    calloc.free(value);
  }
}
