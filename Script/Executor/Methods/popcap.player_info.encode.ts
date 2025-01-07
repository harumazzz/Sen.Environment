namespace Sen.Script.Executor.Methods.PopCap.PlayerInfo.Encode {
	// Using platform

	// export type Platform = Kernel.Support.PopCap.PlayerInfo.Platform;

	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		destination?: string;
		//  platform?: Platform;
	}

	/**
	 * Argument for batch method
	 */

	export interface BatchArgument extends Executor.Base {
		directory: string;
		// platform?: Platform;
	}

	/**
	 * Configuration file if needed
	 */

	export interface Configuration extends Executor.Configuration {}

	/**
	 * Detail namespace
	 */

	/*
    export namespace Detail {
        /**
         * Platform supported
         */

	export const _platform: Array<Kernel.Support.PopCap.Particles.Platform> = [
		'pc',
		'game-console',
		'phone-32',
		'phone-64',
		'tv',
	];
	/**
         *
         * Typical Style
         *
         *

        export function platform(): Array<[bigint, string, string]> {
            return _platform.map((e, i) => [BigInt(i + 1), e as string, Kernel.Language.get(`popcap.reanim.platform.${e}`)]);
        }
    }

    */

	/**
	 * ----------------------------------------------
	 * JavaScript forward method, this method need
	 * to be evaluated during script loading time
	 * ----------------------------------------------
	 */

	export function forward(): void {
		return push_as_module<
			Methods.PopCap.PlayerInfo.Encode.Argument,
			Methods.PopCap.PlayerInfo.Encode.BatchArgument,
			Methods.PopCap.PlayerInfo.Encode.Configuration
		>({
			id: 'popcap.player_info.encode',
			configuration_file: Home.query(
				'~/Executor/Configuration/popcap.player_info.encode.json',
			),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.PlayerInfo.encode_fs(argument.source, argument.destination!);
				clock.stop_safe();
				return;
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)\.dat\.json$/i],
			option: 32n,
		});
		return;
	}
}

Sen.Script.Executor.Methods.PopCap.PlayerInfo.Encode.forward();
