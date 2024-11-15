namespace Sen.Script.Executor.Methods.PvZ2.SCG.Decode {
    /**
     * Argument for the current method
     */
    export type Generic = Sen.Script.Support.Miscellaneous.Custom.StreamCompressedGroup.Configuration.Generic;

    export interface Argument extends Sen.Script.Executor.Base {
        source: string;
        destination?: string;
        generic?: Generic;
        animation_split_label?: boolean;
        enable_debug?: boolean;
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

    export interface Configuration extends Sen.Script.Executor.Configuration {
        generic: 1n | 2n | 3n | "?";
    }

    /**
     * Detail namespace
     */

    export namespace Detail {
        export function generic(): Array<[bigint, bigint, string]> {
            return [
                [1n, 1n, Kernel.Language.get("sen.scg.regular")],
                [2n, 2n, Kernel.Language.get("sen.scg.for_modding")],
                [3n, 3n, Kernel.Language.get("debug")],
            ];
        }
    }

    export interface Configuration extends Sen.Script.Executor.Configuration {}

    /**
     * ----------------------------------------------
     * JavaScript forward method, this method need
     * to be evaluated during script loading time
     * ----------------------------------------------
     */

    export function forward(): void {
        Sen.Script.Executor.push_as_module<
            Sen.Script.Executor.Methods.PvZ2.SCG.Decode.Argument,
            Sen.Script.Executor.Methods.PvZ2.SCG.Decode.BatchArgument,
            Sen.Script.Executor.Methods.PvZ2.SCG.Decode.Configuration
        >({
            id: "pvz2.scg.decode",
            configuration_file: Home.query("~/Executor/Configuration/pvz2.scg.decode.json"),
            direct_forward(argument: Argument): void {
                is_valid_source(argument, false);
                Console.obtained(argument.source);
                defined_or_default<Argument, string>(argument, "destination", `${Kernel.Path.except_extension(argument.source)}.package`);
                check_overwrite(argument as { destination: string }, "directory");
                Console.output(argument.destination!);
                load_boolean(argument, "enable_debug", this.configuration, Kernel.Language.get("pvz2.scg.enable_debug"));
                const generic = Detail.generic();
                if (!argument.enable_debug) {
                    generic.pop();
                }
                load_bigint(argument, "generic", this.configuration, generic, Kernel.Language.get("pvz2.scg.decode.generic"));
                argument.generic! -= 1n;
                if (argument.generic! === 1n) {
                    load_boolean(argument, "animation_split_label", this.configuration, Kernel.Language.get("pvz2.scg.animation_split_label"));
                }
                const setting: Script.Support.Miscellaneous.Custom.StreamCompressedGroup.Configuration.Setting = {
                    decode_method: argument.generic!,
                    animation_split_label: argument.animation_split_label! ?? false,
                };
                clock.start_safe();
                Kernel.Support.Miscellaneous.Custom.StreamCompressedGroup.decode_fs(argument.source, argument.destination!, setting);
                clock.stop_safe();
                return;
            },
            batch_forward(argument: BatchArgument): void {
                return basic_batch(this, argument, false);
            },
            is_enabled: true,
            configuration: undefined!,
            filter: ["file", /(.*)\.scg$/i],
            option: 73n,
        });
        return;
    }
}

Sen.Script.Executor.Methods.PvZ2.SCG.Decode.forward();
