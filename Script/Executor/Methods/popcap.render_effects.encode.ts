namespace Sen.Script.Executor.Methods.PopCap.RenderEffects.Encode {
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
            Sen.Script.Executor.Methods.PopCap.RenderEffects.Encode.Argument,
            Sen.Script.Executor.Methods.PopCap.RenderEffects.Encode.BatchArgument,
            Sen.Script.Executor.Methods.PopCap.RenderEffects.Encode.Configuration
        >({
            id: "popcap.render_effects.encode",
            configuration_file: Home.query("~/Executor/Configuration/popcap.render_effects.encode.json"),
            direct_forward(argument: Argument): void {
                is_valid_source(argument, false);
                Console.obtained(argument.source);
                defined_or_default<Argument, string>(argument, "destination", `${Kernel.Path.except_extension(argument.source)}.popfx`);
                check_overwrite(argument as { destination: string }, "file");
                Console.output(argument.destination!);
                clock.start_safe();
                Kernel.Support.PopCap.RenderEffects.encode_fs(argument.source, argument.destination!);
                clock.stop_safe();
                return;
            },
            batch_forward(argument: BatchArgument): void {
                return basic_batch(this, argument, false);
            },
            is_enabled: true,
            configuration: undefined!,
            filter: ["file", /(.+)\.popfx\.json$/i],
            option: 45n,
        });
        return;
    }
}

Sen.Script.Executor.Methods.PopCap.RenderEffects.Encode.forward();
