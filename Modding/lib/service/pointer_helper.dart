import 'dart:ffi';
import 'dart:typed_data';
import 'package:ffi/ffi.dart';
import 'package:sen/model/api.dart';
import 'dart:convert' as convert;

class PointerHelper {
  static String toDartString(Pointer<CStringView> str) {
    final charCodes = str.ref.value.cast<Uint8>().asTypedList(str.ref.size);
    return convert.utf8.decode(charCodes.sublist(0, charCodes.length));
  }

  static String fromNativeToDartString(CStringView str) {
    final charCodes = str.value.cast<Uint8>().asTypedList(str.size);
    return convert.utf8.decode(charCodes.sublist(0, charCodes.length));
  }

  static List<String> toList(CStringList list) {
    return List<String>.generate(list.size, (i) => fromNativeToDartString(list.value[i]));
  }

  static Pointer<Uint8> utf8ListToCString(Uint8List units) {
    final result = calloc<Uint8>(units.length + 1);
    final list = result.asTypedList(units.length + 1);
    list.setAll(0, units);
    list[units.length] = 0;
    return result.cast<Uint8>();
  }

  static Uint8List toUint8List(String str) {
    return convert.utf8.encode(str);
  }
}
