namespace Sen.Script.Executor.Methods.Data.Base64.Decode {
    /**
     * Argument for the current method
     */

    export interface Argument extends Sen.Script.Executor.Base {
        source: string;
        destination?: string;
    }

    /**
     * Argument for batch method
     */

    export interface BatchArgument extends Sen.Script.Executor.Base {
        directory: string;
    }

    /**
     * Configuration file if needed
     */

    export interface Configuration extends Sen.Script.Executor.Configuration {}

    /**
     * ----------------------------------------------
     * JavaScript forward method, this method need
     * to be evaluated during script loading time
     * ----------------------------------------------
     */

    export function forward(): void {
        Sen.Script.Executor.push_as_module<
            Sen.Script.Executor.Methods.Data.Base64.Decode.Argument,
            Sen.Script.Executor.Methods.Data.Base64.Decode.BatchArgument,
            Sen.Script.Executor.Methods.Data.Base64.Decode.Configuration
        >({
            id: "data.base64.decode",
            configuration_file: Home.query("~/Executor/Configuration/data.base64.decode.json"),
            direct_forward(argument: Argument): void {
                is_valid_source(argument, false);
                Console.obtained(argument.source);
                defined_or_default<Argument, string>(argument, "destination", Kernel.Path.resolve(`${argument.source}.bin`));
                check_overwrite(argument as { destination: string }, "file");
                Console.output(argument.destination!);
                clock.start_safe();
                Kernel.Encryption.Base64.decode_fs(argument.source, argument.destination!);
                clock.stop_safe();
                return;
            },
            batch_forward(argument: BatchArgument): void {
                return basic_batch(this, argument, false);
            },
            is_enabled: false,
            configuration: undefined!,
            filter: ["file", /.*/],
            option: 3n,
        });
        return;
    }
}

Sen.Script.Executor.Methods.Data.Base64.Decode.forward();
