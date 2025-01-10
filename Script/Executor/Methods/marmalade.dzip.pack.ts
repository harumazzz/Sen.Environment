namespace Sen.Script.Executor.Methods.Marmalade.DZip.Pack {
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
			Methods.Marmalade.DZip.Pack.Argument,
			Methods.Marmalade.DZip.Pack.BatchArgument,
			Methods.Marmalade.DZip.Pack.Configuration
		>({
			id: 'marmalade.dzip.pack',
			configuration_file: Home.query('~/Executor/Configuration/marmalade.dzip.pack.json'),
			direct_forward(argument: Argument): void {
				is_valid_source(argument, true);
				Console.obtained(argument.source);
				defined_or_default<Argument, string>(
					argument,
					'destination',
					`${Kernel.Path.except_extension(argument.source)}.dz`,
				);
				check_overwrite(argument as { destination: string }, 'file');
				Console.output(argument.destination!);
				clock.start_safe();
				Kernel.Support.Marmalade.DZip.pack_fs(argument.source, argument.destination!);
				clock.stop_safe();
			},
			is_enabled: true,
			configuration: undefined!,
			filter: ['directory', /(.*)\.data_package$/i],
			option: 6n,
		});
	}
}

Sen.Script.Executor.Methods.Marmalade.DZip.Pack.forward();
