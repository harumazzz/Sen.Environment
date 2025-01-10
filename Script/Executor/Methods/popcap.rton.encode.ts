namespace Sen.Script.Executor.Methods.PopCap.RTON.Encode {
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
			Methods.PopCap.RTON.Encode.Argument,
			Methods.PopCap.RTON.Encode.BatchArgument,
			Methods.PopCap.RTON.Encode.Configuration
		>({
			id: 'popcap.rton.encode',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rton.encode.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.rton`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.RTON.encode_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},
			batch_forward(argument: BatchArgument): void {
				return basic_batch(this, argument, false);
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.+)\.json$/i],
			option: 68n,
		});
	}
}

Sen.Script.Executor.Methods.PopCap.RTON.Encode.forward();
