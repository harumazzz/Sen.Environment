namespace Sen.Script.Executor.Methods.PopCap.Animation.Decode {
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
            Sen.Script.Executor.Methods.PopCap.Animation.Decode.Argument,
            Sen.Script.Executor.Methods.PopCap.Animation.Decode.BatchArgument,
            Sen.Script.Executor.Methods.PopCap.Animation.Decode.Configuration
        >({
            id: "popcap.animation.decode",
            configuration_file: Home.query("~/Executor/Configuration/popcap.animation.decode.json"),
            direct_forward(argument: Argument): void {
                is_valid_source(argument, false);
                Console.obtained(argument.source);
                defined_or_default<Argument, string>(argument, "destination", `${argument.source}.json`);
                check_overwrite(argument as { destination: string }, "file");
                Console.output(argument.destination!);
                Executor.clock.start_safe();
                Kernel.Support.PopCap.Animation.decode_fs(argument.source, argument.destination!);
                Executor.clock.stop_safe();
                return;
            },
            batch_forward(argument: BatchArgument): void {
                return basic_batch(this, argument, false);
            },
            is_enabled: true,
            configuration: undefined!,
            filter: ["file", /(.+)\.pam$/i],
            option: 8n,
        });
        return;
    }
}

Sen.Script.Executor.Methods.PopCap.Animation.Decode.forward();
