namespace Sen.Script.Executor.Methods.PopCap.ResourceGroup.Split {
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
            Sen.Script.Executor.Methods.PopCap.ResourceGroup.Split.Argument,
            Sen.Script.Executor.Methods.PopCap.ResourceGroup.Split.BatchArgument,
            Sen.Script.Executor.Methods.PopCap.ResourceGroup.Split.Configuration
        >({
            id: "popcap.resource_group.split",
            configuration_file: Home.query("~/Executor/Configuration/popcap.resource_group.split.json"),
            direct_forward(argument: Argument): void {
                is_valid_source(argument, false);
                Console.obtained(argument.source);
                defined_or_default<Argument, string>(argument, "destination", `${argument.source}.info`);
                check_overwrite(argument as { destination: string }, "directory");
                Console.output(argument.destination!);
                clock.start_safe();
                Kernel.Support.PopCap.ResourceGroup.split_fs(argument.source, argument.destination!);
                clock.stop_safe();
                return;
            },
            is_enabled: true,
            configuration: undefined!,
            filter: ["file", /(.+)\.json$/i],
            option: 51n,
        });
        return;
    }
}

Sen.Script.Executor.Methods.PopCap.ResourceGroup.Split.forward();
