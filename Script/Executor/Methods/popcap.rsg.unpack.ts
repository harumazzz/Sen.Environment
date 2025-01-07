namespace Sen.Script.Executor.Methods.PopCap.RSG.Unpack {
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
			Methods.PopCap.RSG.Unpack.Argument,
			Methods.PopCap.RSG.Unpack.BatchArgument,
			Methods.PopCap.RSG.Unpack.Configuration
		>({
			id: 'popcap.rsg.unpack',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rsg.unpack.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, false);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.packet`,
				);
				check_overwrite(argument as { destination: string }, 'directory');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.RSG.unpack_fs(argument.source, argument.destination!);
				clock.stop_safe();
				return;
			},
			batch_forward(argument: BatchArgument): void {
				return basic_batch(this, argument, false);
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['file', /(.*)\.rsg$/i],
			option: 63n,
		});
		return;
	}
}

Sen.Script.Executor.Methods.PopCap.RSG.Unpack.forward();
