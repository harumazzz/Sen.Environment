namespace Sen.Script.Executor.Methods.PopCap.Animation.FromFlash {
    /**
     * Argument for the current method
     */

    export interface Argument extends Sen.Script.Executor.Base {
        source: string;
        destination?: string;
        has_label?: boolean;
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
            Sen.Script.Executor.Methods.PopCap.Animation.FromFlash.Argument,
            Sen.Script.Executor.Methods.PopCap.Animation.FromFlash.BatchArgument,
            Sen.Script.Executor.Methods.PopCap.Animation.FromFlash.Configuration
        >({
            id: "popcap.animation.from_flash",
            configuration_file: Home.query("~/Executor/Configuration/popcap.animation.from_flash.json"),
            direct_forward(argument: Argument): void {
                is_valid_source(argument, true);
                Console.obtained(argument.source);
                if (/(.+)\.xfl$/i.test(argument.source)) {
                    defined_or_default<Argument, string>(argument, "destination", `${Kernel.Path.except_extension(argument.source)}.json`);
                } else {
                    defined_or_default<Argument, string>(argument, "destination", `${argument.source}.pam.json`);
                }
                check_overwrite(argument as { destination: string }, "file");
                Console.output(argument.destination!);
                load_boolean(argument, "has_label", this.configuration, Kernel.Language.get("popcap.animation.extract_label"));
                clock.start_safe();
                Kernel.Support.PopCap.Animation.FromFlash.convert_fs(argument.source, argument.destination!, argument.has_label!);
                clock.stop_safe();
                return;
            },
            batch_forward(argument: BatchArgument): void {
                return basic_batch(this, argument, true);
            },
            is_enabled: true,
            configuration: undefined!,
            filter: ["directory", /.*/g],
            option: 12n,
        });
        return;
    }
}

Sen.Script.Executor.Methods.PopCap.Animation.FromFlash.forward();
