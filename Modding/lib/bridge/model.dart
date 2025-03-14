import 'dart:ffi' as ffi;

final class Message extends ffi.Struct {
  external ffi.Pointer<ffi.Uint8> value;

  @ffi.Size()
  external int size;
}

final class Service extends ffi.Struct {
  external ffi.Pointer<ffi.NativeFunction<NativeCallback>> callback;

  external ffi.Pointer<ffi.NativeFunction<NativeAllocate>> allocate;
}

typedef NativeCallback =
    ffi.Void Function(
      ffi.Pointer<Service> service,
      ffi.Pointer<Message> source,
      ffi.Pointer<Message> destination,
    );

typedef ReflectionCallback =
    void Function(
      ffi.Pointer<Service> service,
      ffi.Pointer<Message> source,
      ffi.Pointer<Message> destination,
    );

typedef NativeAllocate =
    ffi.Int32 Function(ffi.Pointer<Message> source, ffi.Pointer<ffi.Size> size);

typedef ReflectionAllocate =
    int Function(ffi.Pointer<Message> source, ffi.Pointer<ffi.Size> size);

typedef NativeExecute =
    ffi.Int32 Function(
      ffi.Pointer<Message> arguments,
      ffi.Pointer<Service> service,
    );

typedef ReflectionExecute =
    int Function(ffi.Pointer<Message> arguments, ffi.Pointer<Service> service);
