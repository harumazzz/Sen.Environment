namespace Sen.Script.Executor.Methods.WWise.Media.Decode {
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
            Sen.Script.Executor.Methods.WWise.Media.Decode.Argument,
            Sen.Script.Executor.Methods.WWise.Media.Decode.BatchArgument,
            Sen.Script.Executor.Methods.WWise.Media.Decode.Configuration
        >({
            id: "wwise.media.decode",
            configuration_file: Home.query("~/Executor/Configuration/wwise.media.decode.json"),
            direct_forward(argument: Argument): void {
                is_valid_source(argument, false);
                Console.obtained(argument.source);
                defined_or_default<Argument, string>(argument, "destination", `${Kernel.Path.except_extension(argument.source)}.wav`);
                check_overwrite(argument as { destination: string }, "file");
                Console.output(argument.destination!);
                clock.start_safe();
                Sen.Script.Support.Wwise.Media.Decode.process_fs(argument.source, argument.destination!);
                clock.stop_safe();
                return;
            },
            is_enabled: true,
            configuration: undefined!,
            filter: ["file", /(.+)\.wem$/i],
            option: 75n,
        });
        return;
    }
}

Sen.Script.Executor.Methods.WWise.Media.Decode.forward();
