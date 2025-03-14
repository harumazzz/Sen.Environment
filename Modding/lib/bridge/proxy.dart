import 'dart:convert';
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart' as ffi;
import 'model.dart';

class Proxy {
  static int _computeSize(List<String> arguments) {
    var result = ffi.sizeOf<ffi.Uint32>();
    for (final e in arguments) {
      result += ffi.sizeOf<ffi.Uint32>() + e.length;
    }
    return result;
  }

  static ffi.Pointer<Message> constructMessage(List<String> arguments) {
    var result = ffi.calloc<Message>();
    var size = _computeSize(arguments);
    result.ref.size = size;
    result.ref.value = ffi.calloc<ffi.Uint8>(size);
    constructProxy(result, arguments);
    return result;
  }

  static void constructProxy(
    ffi.Pointer<Message> source,
    List<String> arguments,
  ) {
    var currentPointer = source.ref.value;
    currentPointer.cast<ffi.Uint32>().value = arguments.length;
    currentPointer += 4;
    for (final e in arguments) {
      var codec = utf8.encode(e);
      currentPointer.cast<ffi.Uint32>().value = codec.length;
      currentPointer += 4;
      currentPointer.asTypedList(codec.length).setAll(0, codec);
      currentPointer += codec.length;
    }
  }

  static void constructDestination(
    ffi.Pointer<Service> service,
    ffi.Pointer<Message> message,
    List<String> arguments,
  ) {
    final size = _computeSize(arguments);
    final temporary = ffi.calloc<ffi.Size>();
    temporary.value = size;
    service.ref.allocate.asFunction<ReflectionAllocate>()(message, temporary);
    constructProxy(message, arguments);
  }

  static List<String> _destructProxy(ffi.Pointer<ffi.Uint8> ptr, int count) {
    var result = List<String>.filled(count, '');
    var currentPtr = ptr;
    for (var i = 0; i < count; i++) {
      var size = currentPtr.cast<ffi.Uint32>().value;
      currentPtr += 4;
      var bytes = currentPtr.asTypedList(size);
      result[i] = utf8.decode(bytes);
      currentPtr += size;
    }
    return result;
  }

  static List<String> destructMessage(ffi.Pointer<Message> message) {
    if (message.ref.size == 0) {
      return [];
    }
    var ptr = message.ref.value;
    var currentPtr = ptr;
    var count = currentPtr.cast<ffi.Uint32>().value;
    currentPtr += 4;
    return _destructProxy(currentPtr, count);
  }

  static void freeMessage(ffi.Pointer<Message> message) {
    if (message != ffi.nullptr) {
      ffi.calloc.free(message.ref.value);
      message.ref.value = ffi.nullptr;
      message.ref.size = 0;
      ffi.calloc.free(message);
    }
  }

  static void freeService(ffi.Pointer<Service> service) {
    if (service != ffi.nullptr) {
      ffi.calloc.free(service);
    }
  }
}
