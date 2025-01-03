import 'dart:ffi';
import 'dart:io';
import 'dart:isolate';
import 'package:async/async.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:sen/model/api.dart';
import 'package:sen/provider/setting_provider.dart';
import 'package:sen/screen/shell/helper/command_handler.dart';
import 'package:sen/screen/shell/model/kernel_argument.dart';
import 'package:sen/screen/shell/model/kernel_script.dart';
import 'package:sen/service/pointer_service.dart';

class KernelHelper {
  static SendPort? _sendPort;

  static int _callback(
    Pointer<CStringList> list,
    Pointer<CStringView> destination,
  ) {
    var result = PointerService.toList(list.ref);
    var command = result.removeAt(0);
    final handler = CommandHandler(_sendPort);
    handler.handle(command, result, destination);
    sleep(const Duration(milliseconds: 10));
    return 0;
  }

  static void _initialize(SendPort sendPort) {
    _sendPort = sendPort;
  }

  static void _release() {
    _sendPort = null;
  }

  static String queryKernelPath(WidgetRef ref) {
    final toolChain = ref.read(settingProvider).toolChain;
    if (Platform.isAndroid) {
      return 'libKernel.so';
    }
    if (Platform.isWindows) {
      return '$toolChain/kernel.dll';
    }
    if (Platform.isLinux) {
      return '$toolChain/libKernel.so';
    }
    if (Platform.isMacOS || Platform.isIOS) {
      return '$toolChain/Kernel.dylib';
    }
    throw Exception('unsupported platform');
  }

  static Future<void> run(List<dynamic> data) async {
    final mainSendPort = data[0] as SendPort;
    final kernelPath = data[1];
    _initialize(mainSendPort);
    final dylib = DynamicLibrary.open(kernelPath);
    final finalizer = Finalizer<DynamicLibrary>((dylib) => dylib.close());
    finalizer.attach(mainSendPort, dylib);
    final execute = dylib.lookup<NativeFunction<KernelExecuteCAPI>>('execute').asFunction<KernelExecuteDartAPI>();
    final subReceivePort = ReceivePort();
    final subStreamQueue = StreamQueue<dynamic>(subReceivePort);
    mainSendPort.send(subReceivePort.sendPort);
    final subEvent = await subStreamQueue.next as List<dynamic>;
    final scriptPath = subEvent[0] as String;
    final currentShell = subEvent[1] as String;
    final arguments = subEvent[2] as List<String>;
    final script = KernelScript(scriptPath);
    final argument = KernelArgument(currentShell, kernelPath, scriptPath, arguments);
    execute(script.value, argument.value, Pointer.fromFunction(_callback));
    script.cleanup();
    argument.cleanup();
    mainSendPort.send(null);
    _release();
    dylib.close();
  }
}
