namespace Sen.Script.Executor.Methods.PopCap.RTON.DecryptAndDecode {
    /**
     * Argument for the current method
     */

    export interface Argument extends Sen.Script.Executor.Base {
        source: string;
        destination?: string;
        key?: string;
        iv?: string;
        use_64_bit_variant?: boolean;
    }

    /**
     * Argument for batch method
     */

    export interface BatchArgument extends Sen.Script.Executor.Base {
        directory: string;
        key?: string;
        iv?: string;
        use_64_bit_variant?: boolean;
    }

    /**
     * Configuration file if needed
     */

    export interface Configuration extends Sen.Script.Executor.Configuration {
        key?: string;
        iv?: string;
        use_64_bit_variant?: boolean;
    }

    /**
     * ----------------------------------------------
     * JavaScript forward method, this method need
     * to be evaluated during script loading time
     * ----------------------------------------------
     */

    export function forward(): void {
        Sen.Script.Executor.push_as_module<
            Sen.Script.Executor.Methods.PopCap.RTON.DecryptAndDecode.Argument,
            Sen.Script.Executor.Methods.PopCap.RTON.DecryptAndDecode.BatchArgument,
            Sen.Script.Executor.Methods.PopCap.RTON.DecryptAndDecode.Configuration
        >({
            id: "popcap.rton.decrypt_and_decode",
            configuration_file: Home.query("~/Executor/Configuration/popcap.rton.decrypt_and_decode.json"),
            direct_forward(argument: Argument): void {
                is_valid_source(argument, false);
                Console.obtained(argument.source);
                defined_or_default<Argument, string>(argument, "destination", `${Kernel.Path.except_extension(argument.source)}.json`);
                check_overwrite(argument as { destination: string }, "file");
                Console.output(argument.destination!);
                load_string(argument, "key", this.configuration, Kernel.Language.get("popcap.rton.decrypt.key"));
                load_string(argument, "iv", this.configuration, Kernel.Language.get("popcap.rton.decrypt.iv"));
                clock.start_safe();
                Kernel.Support.PopCap.RTON.decrypt_and_decode_fs(argument.source, argument.destination!, argument.key!, argument.iv!);
                clock.stop_safe();
                return;
            },
            batch_forward(argument: BatchArgument): void {
                return basic_batch(this, argument, false);
            },
            is_enabled: true,
            configuration: undefined!,
            filter: ["file", /(.+)(\.rton|pp\.dat)$/i],
            option: 65n,
        });
        return;
    }
}

Sen.Script.Executor.Methods.PopCap.RTON.DecryptAndDecode.forward();
