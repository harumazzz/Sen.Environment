import 'dart:collection';
import 'dart:ffi' as ffi;
import 'dart:io';
import 'package:ffi/ffi.dart' as ffi;
import 'dart:isolate';

import 'package:async/async.dart';
import 'client.dart';
import 'model.dart';
import 'proxy.dart';

class Launcher {
  const Launcher._();

  static final Map<ffi.Pointer<Service>, SendPort> _guard =
      HashMap<ffi.Pointer<Service>, SendPort>();

  static Future<void> launch(Client client, List<String> argument) async {
    final mainReceivePort = ReceivePort();
    final mainStreamQueue = StreamQueue<dynamic>(mainReceivePort);
    await client.start();
    await Isolate.spawn(_run, [mainReceivePort.sendPort, argument]);
    final subSendPort = await mainStreamQueue.next as SendPort;
    final service = ffi.calloc<Service>();
    final message = Proxy.constructMessage(argument);
    subSendPort.send([service.address, message.address]);
    while (await mainStreamQueue.hasNext) {
      var mainEvent = await mainStreamQueue.next as List<dynamic>?;
      if (mainEvent == null) {
        mainEvent = await mainStreamQueue.next as List<dynamic>;
        break;
      } else {
        final state = ffi.Pointer<ffi.Bool>.fromAddress(mainEvent[0]);
        final service = ffi.Pointer<Service>.fromAddress(mainEvent[1]);
        final source = ffi.Pointer<Message>.fromAddress(mainEvent[2]);
        final destination = ffi.Pointer<Message>.fromAddress(mainEvent[3]);
        final sourceValue = Proxy.destructMessage(source);
        final result = await client.callback(sourceValue);
        Proxy.constructDestination(service, destination, result);
        state.value = true;
      }
    }
    await client.finish();
    await mainStreamQueue.cancel();
  }

  static Future<void> _run(List<dynamic> args) async {
    final mainSendPort = args[0] as SendPort;
    final arguments = args[1] as List<String>;
    final dylib = ffi.DynamicLibrary.open(arguments[1]);
    var symbol = dylib.lookup<ffi.NativeFunction<NativeExecute>>('execute');
    final execute = symbol.asFunction<ReflectionExecute>();
    final subReceivePort = ReceivePort();
    final subStreamQueue = StreamQueue<dynamic>(subReceivePort);
    mainSendPort.send(subReceivePort.sendPort);
    final subEvent = await subStreamQueue.next as List<dynamic>;
    final service = ffi.Pointer<Service>.fromAddress(subEvent[0] as int);
    final message = ffi.Pointer<Message>.fromAddress(subEvent[1] as int);
    service.ref.callback = ffi.Pointer.fromFunction(callback);
    service.ref.allocate = ffi.nullptr;
    _guard[service] = mainSendPort;
    var result = execute.call(message, service);
    if (result != 0) {
      throw Exception();
    }
    service.ref.allocate = ffi.nullptr;
    _guard.remove(service);
    Proxy.freeMessage(message);
    Proxy.freeService(service);
    symbol = ffi.nullptr;
    dylib.close();
    await subStreamQueue.cancel();
    mainSendPort.send(null);
    mainSendPort.send([]);
  }

  static Future<void> callback(
    ffi.Pointer<Service> service,
    ffi.Pointer<Message> source,
    ffi.Pointer<Message> destination,
  ) async {
    final value = _guard[service];
    assert(value != null);
    final state = ffi.calloc<ffi.Bool>();
    state.value = false;
    value!.send([
      state.address,
      service.address,
      source.address,
      destination.address,
    ]);
    while (!state.value) {
      sleep(const Duration(milliseconds: 10));
    }
    ffi.calloc.free(state);
    return;
  }
}
