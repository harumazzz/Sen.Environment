namespace Sen.Script.Executor.Methods.PopCap.RSG.Pack {
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
			Methods.PopCap.RSG.Pack.Argument,
			Methods.PopCap.RSG.Pack.BatchArgument,
			Methods.PopCap.RSG.Pack.Configuration
		>({
			id: 'popcap.rsg.pack',
			configuration_file: Home.query('~/Executor/Configuration/popcap.rsg.pack.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.rsg`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.PopCap.ResourceStreamGroup.pack_fs(
					argument.source,
					argument.destination!,
				);
				clock.stop_safe();
			},
			batch_forward(argument: BatchArgument): void {
				return basic_batch(this, argument, true);
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['directory', /(.*)\.packet$/i],
			option: 62n,
		});
	}
}

Sen.Script.Executor.Methods.PopCap.RSG.Pack.forward();
