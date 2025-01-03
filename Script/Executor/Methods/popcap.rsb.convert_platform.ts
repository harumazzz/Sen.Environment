namespace Sen.Script.Executor.Methods.PopCap.RSB.ToIOS {
    /**
     * Argument for the current method
     */

    export type Generic = 0n | 1n;

    export interface Argument extends Sen.Script.Executor.Base {
        source: string;
        destination?: string;
        generic?: Generic;
    }

    /**
     * Argument for batch method
     */

    export interface BatchArgument extends Sen.Script.Executor.Base {
        directory: string;
    }

    export interface Configuration extends Sen.Script.Executor.Configuration {
        generic: 1n | 2n | "?";
    }

    /**
     * Configuration file if needed
     */

    export interface Configuration extends Sen.Script.Executor.Configuration {}

    /**
     * Detail
     */

    export namespace Detail {
        /**
         * Input resolution are supported covered here
         */

        /*
        export const resolution = (): Array<[bigint, string, string]> => {
            return [
                [1n, Support.PopCap.ResourceStreamBundle.Miscellaneous.ConvertAndroidToIOS.RSBResolutionX[0], Kernel.Language.get("popcap.animation.to_flash.resolution.1536n")],
                [2n, Support.PopCap.ResourceStreamBundle.Miscellaneous.ConvertAndroidToIOS.RSBResolutionX[1], Kernel.Language.get("popcap.animation.to_flash.resolution.768n")],
            ];
        };
        */

        export function generic(): Array<[bigint, bigint, string]> {
            return [
                [1n, 0n, Kernel.Language.get("popcap.rsb.convert_platform.convert_android_to_ios")],
                [2n, 1n, Kernel.Language.get("popcap.rsb.convert_platform.convert_ios_to_android")],
            ];
        }
    }

    /**
     * ----------------------------------------------
     * JavaScript forward method, this method need
     * to be evaluated during script loading time
     * ----------------------------------------------
     */

    export function forward(): void {
        Sen.Script.Executor.push_as_module<
            Sen.Script.Executor.Methods.PopCap.RSB.ToIOS.Argument,
            Sen.Script.Executor.Methods.PopCap.RSB.ToIOS.BatchArgument,
            Sen.Script.Executor.Methods.PopCap.RSB.ToIOS.Configuration
        >({
            id: "popcap.rsb.convert_platform",
            configuration_file: Home.query("~/Executor/Configuration/popcap.rsb.convert_platform.json"),
            direct_forward(argument: Argument): void {
                is_valid_source(argument, false);
                Console.obtained(argument.source);
                defined_or_default<Argument, string>(argument, "destination", `${argument.source}.bundle`);
                check_overwrite(argument as { destination: string }, "file");
                Console.output(argument.destination!);
                load_bigint(argument, "generic", this.configuration, Detail.generic(), Kernel.Language.get("popcap.rsb.convert_platform.platform_convert"));
                clock.start_safe();
                Support.PopCap.ResourceStreamBundle.Miscellaneous.PlatformConverter.process_fs(argument.source, argument.destination!, argument.generic! !== 1n);
                clock.stop_safe();
                return;
            },
            is_enabled: true,
            configuration: undefined!,
            filter: ["file", /(.*)\.(rsb|obb)$/i],
            option: 80n,
        });
        return;
    }
}

Sen.Script.Executor.Methods.PopCap.RSB.ToIOS.forward();
