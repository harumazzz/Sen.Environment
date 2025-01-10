namespace Sen.Script.Executor.Methods.PopCap.RTON.Decode {
	/**
	 * Argument for the current method
	 */

	export interface Argument extends Executor.Base {
		source: string;
		destination?: string;
	}

	/**
	 * Argument for batch method
	 */

	export interface BatchArgument extends Executor.Base {
		directory: string;
	}

	/**
	 * Configuration file if needed
	 */

	export interface Configuration extends Executor.Configuration {}

	/**
	 * ----------------------------------------------
	 * JavaScript forward method, this method need
	 * to be evaluated during script loading time
	 * ----------------------------------------------
	 */

	export function forward(): void {
		return push_as_module<
			Methods.PopCap.RTON.Decode.Argument,
			Methods.PopCap.RTON.Decode.BatchArgument,
			Methods.PopCap.RTON.Decode.Configuration
		>({
			id: 'popcap.rton.decode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rton.decode.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.json`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.RTON.decode_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},
			batch_forward(argument: BatchArgument): void {
				return basic_batch(this, argument, false);
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)(\.rton|pp\.dat)$/i],
			option: 64n,
		});
	}
}

Sen.Script.Executor.Methods.PopCap.RTON.Decode.forward();
